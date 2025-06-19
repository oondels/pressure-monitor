#pragma once
#include <Arduino.h>
#include <vector>
#include "Alert/IAlertDevice.h"

class SecuritySensor;

class Lamp : public VisualAlertDevice
{
public:
  const char *name;
  Lamp(int pin, const char *name, unsigned long blinkInterval = 500);
  void turnOn();
  void reset() override;
  void triggerAlert() override;
  void performBlink() override;
  void update();
  
  static void test();
  static std::vector<Lamp *> lamps;
  static int lampCount;
  static Lamp *getLampByName(const char *lampName);
  static void turnOffLamps();
  static void toggleLeds(float pressure, SecuritySensor *securitySensor);
  
  private:
  int pin;
  bool isOn;
};