#pragma once
#include <Arduino.h>

class SecuritySensor
{
public:
  SecuritySensor(int pin);
  int pin;
  bool alert;
  bool isActive;
  int lastState;
  unsigned long activeTime;
  unsigned long limitActiveTime;
  unsigned long getActiveTime();

  static void watchSensor(SecuritySensor *sensor);
};