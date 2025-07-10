#pragma once

// Enumeração para tipos de alerta
enum class AlertType
{
  VISUAL,
  AUDIO,
  SECURITY,
  COMPOUND
};

// Interface base para todos os dispositivos de alerta
class IAlertDevice
{
protected:
  bool isActive;

public:
  AlertType alertType;
  IAlertDevice(AlertType type) : alertType(type), isActive(false) {}
  virtual ~IAlertDevice() = default;

  // Métodos virtuais puros
  virtual void reset() = 0;
  virtual void triggerAlert() = 0;

  // Métodos comuns
  AlertType getAlertType() const { return alertType; }
  bool getIsActive() const { return isActive; }
  virtual const char *getTypeString() const = 0;
};

// Classe base para dispositivos visuais
class VisualAlertDevice : public IAlertDevice
{
protected:
  unsigned long lastBlinkTime;
  unsigned long blinkInterval;
  bool blinkState;

public:
  VisualAlertDevice(unsigned long interval = 500)
      : IAlertDevice(AlertType::VISUAL), lastBlinkTime(0),
        blinkInterval(interval), blinkState(false) {}

  const char *getTypeString() const override { return "Visual"; }

  // Método virtual para implementações específicas de piscar
  virtual void performBlink() = 0;

protected:
  // Lógica comum de piscar
  void manageBlink()
  {
    if (!blinkState) {
      Serial.println("Blink state is false, returning from manageBlink.");
      return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval)
    {
      performBlink();
      lastBlinkTime = currentMillis;
    }
  }
};

// Classe base para dispositivos sonoros
class AudioAlertDevice : public IAlertDevice
{
protected:
  unsigned long lastBeepTime;
  unsigned long beepInterval;
  bool beepState;

public:
  AudioAlertDevice(unsigned long interval = 500)
      : IAlertDevice(AlertType::AUDIO), lastBeepTime(0),
        beepInterval(interval), beepState(false) {}

  const char *getTypeString() const override { return "Audio"; }

  // Método virtual para implementações específicas de beep
  virtual void performBeep() = 0;

protected:
  // Lógica comum de beep
  void manageBeep()
  {
    if (!beepState)
      return;

    unsigned long currentMillis = millis();
    if (currentMillis - lastBeepTime >= beepInterval)
    {
      performBeep();
      lastBeepTime = currentMillis;
    }
  }
};

// Classe base para sensores de segurança
class SecurityAlertDevice : public IAlertDevice
{
protected:
  bool alertTriggered;
  unsigned long alertStartTime;

public:
  SecurityAlertDevice()
      : IAlertDevice(AlertType::SECURITY), alertTriggered(false), alertStartTime(0) {}

  const char *getTypeString() const override { return "Security"; }

  // Métodos específicos para sensores
  virtual bool isAlert() const { return alertTriggered; }
  virtual void setAlert(bool alert)
  {
    alertTriggered = alert;
    if (alert && alertStartTime == 0)
    {
      alertStartTime = millis();
    }
    else if (!alert)
    {
      alertStartTime = 0;
    }
  }

  unsigned long getAlertDuration() const
  {
    return alertTriggered ? (millis() - alertStartTime) : 0;
  }
};

// Sistema composto de alertas
class CompoundAlert : public IAlertDevice
{
private:
  IAlertDevice **devices;
  size_t count;

public:
  CompoundAlert(IAlertDevice **devices, size_t count)
      : IAlertDevice(AlertType::COMPOUND), devices(devices), count(count) {}

  const char *getTypeString() const override { return "Compound"; }

  void reset() override
  {
    isActive = false;
    for (size_t i = 0; i < count; i++)
    {
      // Serial.println("Reseting for device: " + String(devices[i]->getTypeString()));
      devices[i]->reset();
    }
  }

  void triggerAlert() override
  {
    isActive = true;
    for (size_t i = 0; i < count; i++)
    {
      // Serial.println("Triggering alert for device: " + String(devices[i]->getTypeString()));
      devices[i]->triggerAlert();
    }
  }

  // Métodos de utilidade
  size_t getDeviceCount() const { return count; }
  IAlertDevice *getDevice(size_t index) const
  {
    return (index < count) ? devices[index] : nullptr;
  }
};