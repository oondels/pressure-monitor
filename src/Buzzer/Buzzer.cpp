#include "Buzzer.h"

Buzzer::Buzzer(int pin, unsigned long beepInterval)
    : AudioAlertDevice(beepInterval), pin(pin), buzzerState(false)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
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
  this->isActive = false;
  this->lastBeepTime = 0;
  this->buzzerState = false;
  this->turnOff();
}

void Buzzer::turnOn()
{
  digitalWrite(pin, LOW);
  buzzerState = true;
}

void Buzzer::turnOff()
{
  digitalWrite(pin, HIGH);
  buzzerState = false;
}

void Buzzer::triggerAlert()
{
  this->isActive = true;
  this->beepState = true;
}

void Buzzer::performBeep()
{
  buzzerState = !buzzerState;
  digitalWrite(this->pin, buzzerState ? LOW : HIGH);
}

void Buzzer::update()
{
  if (this->isActive && this->beepState)
  {
    this->manageBeep();
  }
}