#pragma once
#include <Arduino.h>
#include "../SecuritySensor/SecuritySensor.h"

class Buzzer
{
public:
  Buzzer(int pin);
  void turnOn();
  void turnOff();
  void test();
  void beepBuzzer(float pressure, SecuritySensor *securitySensor);
  bool beepState;

private:
  int pin;
  bool isOn;
  unsigned long lastBeepTime;
  unsigned long beepInterval;
};