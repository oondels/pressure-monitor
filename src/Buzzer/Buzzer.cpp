#include "Buzzer.h"

Buzzer::Buzzer(int pin)
    : pin(pin), isOn(false), lastBeepTime(0), beepInterval(500), beepState(false)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void Buzzer::test()
{
  turnOn();
  delay(500);
  turnOff();
}

void Buzzer::turnOn()
{
  this->isOn = true;
  digitalWrite(pin, HIGH);
}

void Buzzer::turnOff()
{
  if (!this->beepState)
  {
    this->isOn = false;
    digitalWrite(pin, LOW);
  }
}

void Buzzer::beepBuzzer(float pressure, bool securityAlert)
{
  if (pressure < 2.0 || securityAlert)
  {
    unsigned long currentMillis = millis();

    this->beepState = true;
    if (currentMillis - lastBeepTime >= beepInterval)
    {
      this->isOn = !this->isOn;
      digitalWrite(this->pin, isOn ? HIGH : LOW);
      lastBeepTime = currentMillis;
    }
    return;
  }

  this->beepState = false;
  this->turnOff();
}