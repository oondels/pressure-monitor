#pragma once
#include <Arduino.h>
#include "SecuritySensor/SecuritySensor.h"
#include "Alert/IAlertDevice.h"

class Buzzer : public IAlertDevice
{
public:
  Buzzer(int pin);
  void turnOn();
  void turnOff();
  void test();
  void reset() override;
  void triggerAlert() override;
  bool beepState;


private:
  int pin;
  bool isOn;
  unsigned long lastBeepTime;
  unsigned long beepInterval;
  void beepBuzzer();
};