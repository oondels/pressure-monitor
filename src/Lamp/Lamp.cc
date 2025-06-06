#include "Lamp.h"

#define LOW_PRESSURE_THRESHOLD 2.0
#define HIGH_PRESSURE_THRESHOLD 5.0
#define BLINK_INTERVAL 500

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

void Lamp::blinkAlert(bool securityAlert)
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= blinkInterval && this->blinkState)
  {
    this->isOn = this->isOn == 0 ? HIGH : LOW;
    digitalWrite(this->pin, this->isOn == 0 ? HIGH : LOW);
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
  Lamp *searchedLamp = nullptr;
  for (Lamp *lamp : lamps)
  {
    if (strcmp(lamp->name, lampName) == 0)
    {
      searchedLamp = lamp;
      break;
    }
  }

  return searchedLamp;
}

void Lamp::toggleLeds(float pressure, SecuritySensor *securitySensor)
{
  Lamp::turnOffLamps();

  // Reset the blink mode
  for (Lamp *lamp : lamps)
  {
    lamp->blinkState = false;
  }

  // Verifica se o alerta do sensor esta ativo, para manter apenas uma lampada acesa
  if (securitySensor->alert)
  {
    Serial.println("Alerta ativo, skipping...");
    return;
  }

  if (pressure <= LOW_PRESSURE_THRESHOLD)
  {
    Lamp *redLamp = getLampByName("Vermelho");
    if (redLamp)
      redLamp->blinkState = true;
    redLamp->blinkAlert();
  }

  else if (pressure > LOW_PRESSURE_THRESHOLD && pressure < HIGH_PRESSURE_THRESHOLD)
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
