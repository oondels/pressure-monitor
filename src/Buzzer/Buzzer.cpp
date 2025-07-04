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

void Buzzer::reset()
{
  this->beepState = false;
  this->turnOff();
  this->lastBeepTime = 0;
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

void Buzzer::triggerAlert()
{
  this->beepState = true;
  this->beepBuzzer();
}

void Buzzer::beepBuzzer()
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