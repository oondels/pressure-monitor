#include "Lamp.h"

int Lamp::lampCount = 0;
std::vector<Lamp *> Lamp::lamps;

Lamp::Lamp(int pin, const char *name) : pin(pin), name(name), isOn(false), lastBlinkTime(0), blinkInterval(500), blinkState(false)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); // Start with the lamp off

  // Register the lamp in the static vector
  lamps.push_back(this);
  lampCount++;
};

void Lamp::test()
{
  for (Lamp *lamp : lamps)
  {
    digitalWrite(lamp->pin, LOW);
    Serial.println(lamp->name);
    delay(500);
    digitalWrite(lamp->pin, HIGH);
  }
}

void Lamp::turnOn()
{
  for (Lamp *lamp : lamps)
  {
    if (lamp == this)
    {
      lamp->isOn = true;
      digitalWrite(lamp->pin, LOW);
      continue;
    }
    lamp->isOn = false;
    digitalWrite(lamp->pin, HIGH);
  }
};
