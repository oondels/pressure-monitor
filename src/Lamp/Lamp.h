#pragma once
#include <Arduino.h>
#include <vector>

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
  static void toggleLeds(float pressure);

private:
  int pin;
  const char* name;
  bool isOn;
  unsigned long lastBlinkTime;
  unsigned long blinkInterval;
  bool blinkState;
};