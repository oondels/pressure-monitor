#include "PressureSensor.h"

float PressureSensor::sensorSignal = 0.0;
float PressureSensor::pressure = 0.0;

PressureSensor::PressureSensor(int pin, float maxPressure, float minPressure)
    : pin(pin), maxPressure(maxPressure), minPressure(minPressure)
{
  pinMode(this->pin, INPUT);
}

void PressureSensor::readSensorSignal(int samples = 10)
{
  long sum = 0;
  for (int i = 0; i < samples; i++)
  {
    sum += analogRead(this->pin);
    delay(10);
  }
  float averageSignal = sum / (float)samples;

  // Convert the signal to Volts
  sensorSignal = (averageSignal / 4095.0) * 3.3;
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float PressureSensor::getPressure()
{
  if (sensorSignal <= 0.0187)
  {
    pressure = 0.0;
  }

  if (sensorSignal <= 0.09296)
  {
    pressure = mapFloat(sensorSignal, 0.01866, 0.09296, 0.0, 1.0);
  }

  if (sensorSignal <= 0.13836)
  {
    pressure = mapFloat(sensorSignal, 0.09296, 0.13836, 1.0, 2.0);
  }
  else if (sensorSignal <= 0.16890)
  {
    pressure = mapFloat(sensorSignal, 0.13836, 0.16890, 2.0, 3.0);
  }
  else if (sensorSignal <= 0.19740)
  {
    pressure = mapFloat(sensorSignal, 0.16890, 0.19740, 3.0, 4.0);
  }
  else if (sensorSignal <= 0.21561)
  {
    pressure = mapFloat(sensorSignal, 0.19740, 0.21561, 4.0, 5.0);
  }
  else if (sensorSignal <= 0.23354)
  {
    pressure = mapFloat(sensorSignal, 0.21561, 0.23354, 5.0, 6.0);
  }
  else if (sensorSignal <= 0.24952)
  {
    pressure = mapFloat(sensorSignal, 0.23354, 0.24952, 6.0, 7.0);
  }
  else if (sensorSignal <= 0.2550)
  {
    pressure = mapFloat(sensorSignal, 0.24952, 0.2550, 7.0, 7.5);
  }
  else
  {
    pressure = 8.0;
  }

  if (pressure < 0.0)
    pressure = 0.0;

  return pressure;
}
