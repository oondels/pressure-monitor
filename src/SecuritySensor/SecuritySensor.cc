#include "SecuritySensor.h"
#include "Buzzer/Buzzer.h"
#include "Lamp/Lamp.h"

#define DELAY_AFTER_ALERT 3000

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

void SecuritySensor::watchSensor(SecuritySensor *sensor, Buzzer *buzzer, Lamp *lamp)
{
  int signal = digitalRead(sensor->pin);

  // sensor->isActive = (signal == HIGH);
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
    // bool isFirstDesactivate = sensor->deactivateTime == 0 && sensor->lastState == HIGH;
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

    //* Log para depuração
    // if (sensor->isActive && sensor->lastState == HIGH)
    // {
    //   Serial.println(sensor->getDeactiveTime());
    // }
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

  // Verifica se tempo passou e nao chegou pressão para ativar o alerta
  // unsigned long timeActive = millis() - sensor->activeTime;
  if (sensor->isActive && sensor->getActiveTime() >= sensor->limitActiveTime)
  {
    // buzzer->beepBuzzer(0.0, true);
    sensor->alert = true;
    lamp->blinkState = true;
    lamp->blinkAlert();
    return;
  }
};