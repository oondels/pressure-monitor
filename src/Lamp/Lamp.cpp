#include "Lamp.h"
#include "SecuritySensor/SecuritySensor.h"

#define LOW_PRESSURE_THRESHOLD 2.0
#define HIGH_PRESSURE_THRESHOLD 5.0
#define BLINK_INTERVAL 500

int Lamp::lampCount = 0;
std::vector<Lamp *> Lamp::lamps;

Lamp::Lamp(int pin, const char *name, unsigned long blinkInterval)
    : VisualAlertDevice(blinkInterval), pin(pin), name(name), isOn(false)
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

// ! Corrigir erro que lampada nao esta piscando em alert mode
void Lamp::triggerAlert()
{
  this->isActive = true;
  this->isOn = true;
  this->blinkState = true;
}

void Lamp::performBlink()
{
  isOn = !isOn;
  digitalWrite(pin, isOn ? LOW : HIGH);
}

// Static Methods
void Lamp::test()
{
  for (Lamp *lamp : lamps)
  {
    digitalWrite(lamp->pin, LOW);
    delay(500);
    digitalWrite(lamp->pin, HIGH);
  }
}

void Lamp::reset()
{
  for (Lamp *lamp : lamps)
  {
    lamp->blinkState = false;
    lamp->isActive = false;
    lamp->isOn = false;
    digitalWrite(lamp->pin, HIGH);
  }
}

void Lamp::turnOffLamps()
{
  for (Lamp *lamp : lamps)
  {
    if (!lamp->blinkState)
    {
      lamp->isActive = false;
      lamp->blinkState = false;
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
    if (lamp->name == "Vermelho" && lamp->isActive)
    {
      Serial.println("Red lamp is active, turning on.");
      return;
    }

    lamp->blinkState = false;
  }

  // Verifica se o alerta do sensor esta ativo, para manter apenas uma lampada acesa
  if (securitySensor->isAlert())
    return;

  if (pressure <= LOW_PRESSURE_THRESHOLD)
  {
    Lamp *redLamp = getLampByName("Vermelho");
    if (redLamp)
    {
      redLamp->turnOn();
    }
  }

  if (pressure > LOW_PRESSURE_THRESHOLD && pressure < HIGH_PRESSURE_THRESHOLD)
  {
    Lamp *yellowLamp = getLampByName("Amarelo");
    if (yellowLamp)
    {
      yellowLamp->turnOn();
      yellowLamp->isActive = true;
    }
  }

  else if (pressure >= HIGH_PRESSURE_THRESHOLD)
  {
    Lamp *greenLamp = getLampByName("Verde");
    if (greenLamp)
      greenLamp->turnOn();
  }
}

void Lamp::update()
{
  if (this->isActive && this->blinkState)
  {
    Serial.println("Managing blink for lamp: " + String(this->name));
    this->manageBlink();
  }
}