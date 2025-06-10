#pragma once
#include <Arduino.h>
#include <vector>
#include "SecuritySensor/SecuritySensor.h"
// Forward declaration - only declares the name, not the full class

class Lamp
{
public:
  const char *name;
  Lamp(int pin, const char *name);
  void turnOn();
  bool blinkState;
  void blinkAlert(bool securityAlert = false);

  static void test();
  static std::vector<Lamp *> lamps;
  static int lampCount;
  static void reset();
  static Lamp *getLampByName(const char *lampName);
  static void turnOffLamps();
  static void toggleLeds(float pressure, SecuritySensor *securitySensor);

private:
  int pin;
  bool isOn;
  unsigned long lastBlinkTime;
  unsigned long blinkInterval;
};