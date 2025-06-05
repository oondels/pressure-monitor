#pragma once
#include <Arduino.h>

class PressureSensor
{
public:
  PressureSensor(int pin, float maxPressure, float minPressure, float operatingPressure);
  float operatingPressure;
  float getPressure();
  void readSensorSignal(int samples);

  float getMaxPressure() const { return maxPressure; }
  float getMinPressure() const { return minPressure; }
  void setMaxPressure(float maxPressure) { this->maxPressure = maxPressure; }
  void setMinPressure(float minPressure) { this->minPressure = minPressure; }

private:
  static float sensorSignal;
  static float pressure;
  int pin;
  float maxPressure;
  float minPressure;
};