#pragma once
#include <Arduino.h>

class Buzzer
{
  public:
    Buzzer(int pin);
    void turnOn();
    void turnOff();
    void test();
  
  private:
    int pin;
    bool isOn;
};