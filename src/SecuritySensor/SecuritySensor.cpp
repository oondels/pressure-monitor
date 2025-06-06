#include "SecuritySensor.h"
#include "Buzzer/Buzzer.h"
#include "Lamp/Lamp.h"
#include "PressureSensor/PressureSensor.h"

#define DELAY_AFTER_ALERT 4000

SecuritySensor::SecuritySensor(int pin)
    : pin(pin), isActive(false), lastState(0), activeTime(0), deactivateTime(0), alert(false), limitActiveTime(3000)
{
  pinMode(this->pin, INPUT);
}

unsigned long SecuritySensor::getActiveTime()
{
  if (!this->isActive)
    return 0;

  return millis() - this->activeTime;
}

unsigned long SecuritySensor::getDeactiveTime()
{
  if (this->isActive && this->lastState == LOW)
    return 0;

  return millis() - this->deactivateTime;
}

void SecuritySensor::watchSensor(SecuritySensor *sensor, Buzzer *buzzer, Lamp *lamp, PressureSensor *pressureSensor)
{
  int signal = digitalRead(sensor->pin);

  // RESET do alerta se o botão for pressionado novamente DURANTE o alerta
  // Detecta borda de subida durante o alerta
  if (sensor->alert && signal == HIGH && sensor->lastSignal == LOW)
  {
    sensor->isActive = false;
    sensor->deactivateTime = 0;
    sensor->alert = false;
    sensor->activeTime = millis();

    sensor->lastSignal = signal; // Atualiza estado
    return;
  }

  // Check se o sinal do sensor esta chegando
  if (signal == HIGH)
  {
    sensor->isActive = true;
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
      sensor->isActive = false;
      sensor->deactivateTime = 0;
      sensor->alert = false;
    }
  }

  // Verifica se o sensor de segurança esta ativo e grava
  // o momento de ativação para comparação futura
  if (sensor->isActive && !sensor->lastState && signal == HIGH)
  {
    sensor->activeTime = millis();
    sensor->lastState = signal;
  }

  // Reseta se o sensor for desativado
  else if (sensor->lastState && !sensor->isActive)
  {
    sensor->activeTime = 0;
    sensor->lastState = !sensor->lastState;
  }

  // Updates the last read signal
  sensor->lastSignal = signal;

  // Verifica se tempo passou e nao chegou pressão para ativar o alerta
  // unsigned long timeActive = millis() - sensor->activeTime;
  if (sensor->isActive && sensor->getActiveTime() >= sensor->limitActiveTime && pressureSensor->getPressure() < 5.0)
  {
    buzzer->beepBuzzer(0.0, true);
    sensor->alert = true;
    lamp->blinkState = true;
    lamp->blinkAlert();
    return;
  }
};