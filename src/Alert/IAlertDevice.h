#pragma once

class IAlertDevice
{
public:
  virtual void reset() = 0;
  virtual void triggerAlert() = 0;
  virtual ~IAlertDevice() = default;
};

class CompoundAlert : public IAlertDevice
{
  IAlertDevice **devices;
  size_t count;

public:
  CompoundAlert(IAlertDevice **devices, size_t count)
      : devices(devices), count(count) {}

  void reset() override
  {
    for (size_t i = 0; i < count; i++)
    {
      devices[i]->reset();
    }
  }
  void triggerAlert() override
  {
    for (size_t i = 0; i < count; i++)
    {
      devices[i]->triggerAlert();
    }
  }
};