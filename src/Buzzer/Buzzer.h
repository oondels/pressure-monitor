#pragma once
#include <Arduino.h>
#include "SecuritySensor/SecuritySensor.h"
#include "Alert/IAlertDevice.h"

class Buzzer : public AudioAlertDevice
{
public:
  Buzzer(int pin, unsigned long beepInterval = 500);
  void turnOn();
  void turnOff();
  void test();
  void reset() override;
  void triggerAlert() override;
  void performBeep() override;
  void update(); // Método para ser chamado no loop principal

private:
  int pin;
  bool buzzerState;
};