#include "SecuritySensor.h"
#include "Buzzer/Buzzer.h"
#include "Lamp/Lamp.h"
#include "PressureSensor/PressureSensor.h"

#define DELAY_AFTER_ALERT 4000

SecuritySensor::SecuritySensor(int pin)
    : pin(pin), isOn(false), lastState(LOW), activeTime(0), deactivateTime(0), alert(false), limitActiveTime(3000), alertTime(0), limitAlertTime(4000), lastAlertState(false)
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

void SecuritySensor::watchSensor(SecuritySensor *sensor, Buzzer *buzzer, Lamp *lamp, PressureSensor *pressureSensor)
{
  int signal = digitalRead(sensor->pin);

  // RESET do alerta se o botão for pressionado novamente DURANTE o alerta
  if (sensor->alert && signal == HIGH && sensor->lastSignal == LOW)
  {
    sensor->activeTime = millis();
    sensor->lastSignal = signal;
    sensor->lastState = signal;

    // Reset all components
    Lamp::reset();
    buzzer->reset();
    sensor->reset();
    return;
  }

  // Verifica se o tempo de alerta passou
  if (sensor->getAlertTime() >= sensor->limitAlertTime)
  {
    sensor->transitionState(SensorState::INACTIVE);
    return;
  }

  // Check se o sinal do sensor esta chegando
  if (signal == HIGH)
  {
    sensor->transitionState(SensorState::ACTIVE);
  }
  else
  {
    /*
      - Verifica se é a primeira desativação e registra o momento (ms)
      - reseta o alerta e tem desativado se:
        - tiver mais de 3 segundos desativado e com alerta
        - ou se o sinal esta desligado e nao tem alerta
    */
    if (sensor->deactivateTime == 0 && sensor->lastState == HIGH)
    {
      sensor->deactivateTime = millis();
    }
    else if (sensor->getDeactiveTime() >= DELAY_AFTER_ALERT || (signal == LOW && !sensor->alert))
    {
      sensor->transitionState(SensorState::INACTIVE);
    }
  }

  // Reseta se o sensor for desativado
  if (sensor->lastState && !sensor->isActive())
  {
    sensor->reset();
    sensor->lastState = !sensor->lastState;
  }

  // Updates the last read signal
  sensor->lastSignal = signal;

  // Verifica se tempo passou e nao chegou pressão para ativar o alerta
  if (sensor->isActive() && sensor->getActiveTime() >= sensor->limitActiveTime && pressureSensor->getPressure() < 5.0)
  {
    sensor->transitionState(SensorState::ALERT);

    buzzer->beepBuzzer(0.0, true);
    lamp->blinkState = true;
    lamp->blinkAlert();
    return;
  }
};