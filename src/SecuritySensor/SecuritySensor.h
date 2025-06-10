#pragma once
#include <Arduino.h>
#include "Buzzer/Buzzer.h"
#include "Lamp/Lamp.h"
#include "PressureSensor/PressureSensor.h"
#include "Alert/IAlertDevice.h"

enum class SensorState {
  INACTIVE,
  ACTIVE,
  ALERT
};

class SecuritySensor
{
private:
  int pin;
  bool alert;
  bool lastAlertState;
  bool isOn;
  int lastState;
  int lastSignal;
  unsigned long deactivateTime;
  unsigned long activeTime;
  unsigned long limitActiveTime;
  unsigned long alertTime;
  unsigned long limitAlertTime;

public:
  SecuritySensor(int pin);
  unsigned long getAlertTime();
  unsigned long getActiveTime();
  unsigned long getDeactiveTime();
  void reset();
  void active();
  void deactivate();
  bool isActive();
  bool isAlert() const { return alert; }
  void transitionState(SensorState newState);
  

  static void watchSensor(IAlertDevice& alertDevice, PressureSensor *pressureSensor, SecuritySensor *securitySensor);
};