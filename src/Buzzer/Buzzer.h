#pragma once
#include <Arduino.h>

class Buzzer
{
public:
  Buzzer(int pin);
  void turnOn();
  void turnOff();
  void test();
  void beepBuzzer(unsigned long pressure);

private:
  int pin;
  bool isOn;
  unsigned long lastBeepTime;
  unsigned long beepInterval;
  bool beepState;
};