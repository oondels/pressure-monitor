#pragma once
#include <Arduino.h>
#include <vector>

class Lamp
{
public:
  Lamp(int pin, const char* name);
  static std::vector<Lamp *> lamps;
  static int lampCount;
  void test();
  void turnOn();
  void turnOff();
  // void blinkAlert(unsigned long interval = 500);

private:
  int pin;
  const char* name;
  bool isOn;
  unsigned long lastBlinkTime;
  unsigned long blinkInterval;
  bool blinkState;
};