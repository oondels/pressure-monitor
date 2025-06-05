#pragma once
#include <Arduino.h>
#include <vector>
#include "../SecuritySensor/SecuritySensor.h"

class Lamp
{
public:
  Lamp(int pin, const char* name);
  static std::vector<Lamp *> lamps;
  static int lampCount;
  void turnOn();
  void blinkAlert();
  
  static void test();
  static Lamp* getLampByName(const char* lampName);
  static void turnOffLamps();
  static void toggleLeds(float pressure, SecuritySensor *securitySensor = nullptr);

private:
  int pin;
  const char* name;
  bool isOn;
  unsigned long lastBlinkTime;
  unsigned long blinkInterval;
  bool blinkState;
};