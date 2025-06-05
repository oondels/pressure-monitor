#include "SecuritySensor.h"

SecuritySensor::SecuritySensor(int pin)
    : pin(pin), isActive(false), lastState(0), activeTime(0), alert(false), limitActiveTime(3000)
{
  pinMode(this->pin, INPUT);
}

unsigned long SecuritySensor::getActiveTime()
{
  if (!this->isActive)
    return 0;

  return millis() - this->activeTime;
}

void SecuritySensor::watchSensor(SecuritySensor *sensor)
{
  int signal = digitalRead(sensor->pin);
  sensor->isActive = (signal == HIGH);

  if (sensor->isActive && !sensor->lastState && signal == HIGH)
  {
    sensor->activeTime = millis();
    sensor->lastState = signal;
  }

  else if (sensor->lastState && !sensor->isActive)
  {
    // Serial.println("Reseting");
    sensor->activeTime = 0;
    sensor->lastState = !sensor->lastState;
    sensor->alert = false;
  }
};