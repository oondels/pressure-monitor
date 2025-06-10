#pragma once
#include <Arduino.h>

class Buzzer
{
public:
  Buzzer(int pin);
  void turnOn();
  void turnOff();
  void test();
  void reset();
  void beepBuzzer(float pressure = 0.0, bool securityAlert = false);
  bool beepState;


private:
  int pin;
  bool isOn;
  unsigned long lastBeepTime;
  unsigned long beepInterval;
};