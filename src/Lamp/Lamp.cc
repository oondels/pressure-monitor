#include "Lamp.h"

int Lamp::lampCount = 0;
std::vector<Lamp *> Lamp::lamps;

Lamp::Lamp(int pin, const char *name)
    : pin(pin), name(name), isOn(false), lastBlinkTime(0), blinkInterval(500), blinkState(false)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); // Start with the lamp off

  // Register the lamp in the static vector
  lamps.push_back(this);
  lampCount++;
};

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

void Lamp::blinkAlert()
{
  unsigned long currentMillis = millis();

  this->blinkState = true;
  if (currentMillis - lastBlinkTime >= blinkInterval)
  {
    this->isOn = !this->isOn;
    digitalWrite(this->pin, this->isOn ? HIGH : LOW);
    lastBlinkTime = currentMillis;
  }
}

// Static Methods
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

void Lamp::turnOffLamps()
{
  for (Lamp *lamp : lamps)
  {
    if (!lamp->blinkState)
    {
      lamp->isOn = false;
      digitalWrite(lamp->pin, HIGH);
    }
  }
}

Lamp *Lamp::getLampByName(const char *lampName)
{
  Lamp *searchedLamp;
  for (Lamp *lamp : lamps)
  {
    if (strcmp(lamp->name, lampName) == 0)
    {
      searchedLamp = lamp;
    }
  }

  return searchedLamp;
}

void Lamp::toggleLeds(float pressure)
{
  Lamp::turnOffLamps();

  // Reset the blink mode
  for (Lamp *lamp : lamps)
  {
    lamp->blinkState = false;
  }

  if (pressure <= 2.0)
  {
    Lamp *redLamp = getLampByName("Vermelho");
    if (redLamp)
      redLamp->blinkAlert();
  }
  else if (pressure > 2.0 && pressure < 5.0)
  {
    Lamp *yellowLamp = getLampByName("Amarelo");
    if (yellowLamp)
      yellowLamp->turnOn();
  }
  else
  {
    Lamp *greenLamp = getLampByName("Verde");
    if (greenLamp)
      greenLamp->turnOn();
  }
}
