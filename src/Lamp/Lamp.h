#pragma once
#include <Arduino.h>
#include <vector>
#include "Alert/IAlertDevice.h"

class SecuritySensor;

class Lamp : public IAlertDevice
{
public:
  const char *name;
  Lamp(int pin, const char *name);
  void turnOn();
  bool blinkState;
  void reset() override;
  void triggerAlert() override;
  
  static void test();
  static std::vector<Lamp *> lamps;
  static int lampCount;
  static Lamp *getLampByName(const char *lampName);
  static void turnOffLamps();
  static void toggleLeds(float pressure, SecuritySensor *securitySensor);
  
  private:
  void blinkAlert();
  int pin;
  bool isOn;
  unsigned long lastBlinkTime;
  unsigned long blinkInterval;
};