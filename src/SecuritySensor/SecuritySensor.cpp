#include "SecuritySensor.h"
#include "PressureSensor/PressureSensor.h"

#define DELAY_AFTER_ALERT 6000

SecuritySensor::SecuritySensor(int pin)
    : pin(pin), isOn(false), lastState(LOW), activeTime(0), deactivateTime(0), alert(false),
      limitActiveTime(3000), alertTime(0), limitAlertTime(4000), lastAlertState(false), alertTriggered(false)
{
  pinMode(this->pin, INPUT);
}

bool SecuritySensor::isActive()
{
  return this->isOn;
}

void SecuritySensor::active()
{
  this->isOn = true;
  this->lastSignal = HIGH;
  this->deactivateTime = 0;
  if (this->isActive() && this->lastState == LOW)
  {
    this->lastState = HIGH;
    this->activeTime = millis();
  }
}

void SecuritySensor::deactivate()
{
  this->isOn = false;
  this->alert = false;
  this->lastAlertState = false;
  this->alertTime = 0;
  this->lastState = LOW;
  this->activeTime = 0;
  if (this->lastSignal == HIGH)
    this->lastSignal = LOW;
  this->deactivateTime = millis();
}

void SecuritySensor::transitionState(SensorState newState)
{
  switch (newState)
  {
  case SensorState::INACTIVE:
    this->deactivate();
    break;
  case SensorState::ACTIVE:
    this->active();
    break;
  case SensorState::ALERT:
    this->deactivateTime = 0;
    this->alert = true;
    this->isOn = true;
    if (!this->lastAlertState)
    {
      this->alertTime = millis();
      this->lastAlertState = true;
    }
    break;
  }
}

void SecuritySensor::reset()
{
  this->alert = false;
  this->alertTime = 0;
  this->lastAlertState = false;
  this->isOn = false;
  this->lastState = LOW;
  this->lastSignal = LOW;
  this->deactivateTime = 0;
  this->activeTime = 0;
  this->alertTriggered = false;
  this->limitActiveTime = 3000; // Reset to default value
}

unsigned long SecuritySensor::getAlertTime()
{
  if (!this->isAlert())
    return 0;

  return millis() - this->alertTime;
}

unsigned long SecuritySensor::getActiveTime()
{
  if (!this->isActive())
    return 0;

  return millis() - this->activeTime;
}

unsigned long SecuritySensor::getDeactiveTime()
{
  if (this->isActive() && this->lastState == LOW || this->deactivateTime == 0)
    return 0;

  return millis() - this->deactivateTime;
}

void SecuritySensor::watchSensor(IAlertDevice &alertSystem, PressureSensor *pressureSensor, SecuritySensor *securitySensor)
{
  int signal = digitalRead(securitySensor->pin);

  // RESET do alerta se o botão for pressionado novamente DURANTE o alerta
  if (securitySensor->alert && signal == HIGH && securitySensor->lastSignal == LOW)
  {
    securitySensor->activeTime = millis();
    securitySensor->lastSignal = signal;
    securitySensor->lastState = signal;

    // Reset all components
    alertSystem.reset();
    securitySensor->reset();
    return;
  }

  // * Retirei o delay para que o alarme se mantenha acionado enquanto o sensor estiver ativo -> a limitação de tempo será apenas no Buzzer
  // Verifica se o tempo de alerta passou
  if (securitySensor->getAlertTime() >= securitySensor->limitAlertTime)
  {
    securitySensor->transitionState(SensorState::INACTIVE);
    return;
  }

  // Check se o sinal do sensor esta chegando
  if (signal == HIGH)
  {
    securitySensor->transitionState(SensorState::ACTIVE);
  }
  else
  {
    /*
      - Verifica se é a primeira desativação e registra o momento (ms)
      - reseta o alerta se o sensor esta ativo e nao tem alerta
    */
    if (securitySensor->deactivateTime == 0 && securitySensor->lastState == HIGH)
    {
      securitySensor->deactivateTime = millis();
    }
    else if (!securitySensor->alert && securitySensor->isActive())
    {
      securitySensor->reset();
    }
  }

  // Updates the last read signal
  securitySensor->lastSignal = signal;

  // Verifica se tempo passou e nao chegou pressão para ativar o alerta
  if (securitySensor->isActive() && securitySensor->getActiveTime() >= securitySensor->limitActiveTime && pressureSensor->getPressure() < 5.0)
  {
    securitySensor->transitionState(SensorState::ALERT);
    

    // Só aciona o alarme se ainda não foi acionado nesta ativação
    if (!securitySensor->alertTriggered)
    {
      Serial.println("Alert triggered by SecuritySensor.");
      alertSystem.triggerAlert();
      securitySensor->alertTriggered = true; // Marca que o alarme já foi acionado
    }
    return;
  }
};