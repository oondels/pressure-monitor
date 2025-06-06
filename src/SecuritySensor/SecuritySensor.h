#pragma once
#include <Arduino.h>

class Buzzer; 
class Lamp; 

class SecuritySensor
{
public:
  SecuritySensor(int pin);
  int pin;
  bool alert;
  bool isActive;
  int lastState;
  unsigned long deactivateTime;
  unsigned long activeTime;
  unsigned long limitActiveTime;
  unsigned long getActiveTime();
  unsigned long getDeactiveTime();

  static void watchSensor(SecuritySensor *sensor, Buzzer *buzzer, Lamp *lamp);
};