#include "Lamp/Lamp.h"
#include "Buzzer/Buzzer.h"
#include "PressureSensor/PressureSensor.h"
#include "SecuritySensor/SecuritySensor.h"
#include "MessageManager/Message.h"

#define GREEN_LAMP 26
#define YELLOW_LAMP 25
#define RED_LAMP 27
#define BUZZER_PIN 14
#define SECURITY_PIN 12

#define SENSOR_PIN 34
float max_pressure = 8.0;
float min_pressure = 2.0;
float operatingPressure = 5.0;

Message *messageManager;
SecuritySensor *securitySensor;
PressureSensor *pressureSensor;
Buzzer *buzzer;
Lamp *greenLamp;
Lamp *yellowLamp;
Lamp *redLamp;

void setup()
{
  Serial.begin(115200);

  Serial.println("Initializing...");
  delay(1000);

  messageManager = new Message(1000);
  pressureSensor = new PressureSensor(SENSOR_PIN, max_pressure, min_pressure, operatingPressure);
  securitySensor = new SecuritySensor(SECURITY_PIN);
  buzzer = new Buzzer(BUZZER_PIN);
  greenLamp = new Lamp(GREEN_LAMP, "Verde");
  yellowLamp = new Lamp(YELLOW_LAMP, "Amarelo");
  redLamp = new Lamp(RED_LAMP, "Vermelho");

  Serial.println("Testing components...");
  Lamp::test();
  buzzer->test();
}

void loop()
{
  SecuritySensor::watchSensor(securitySensor, buzzer, redLamp, pressureSensor);

  // Check the sensor
  if (securitySensor->isActive)
  {
    int samples = 20;
    pressureSensor->readSensorSignal(samples);
    float pressure = pressureSensor->getPressure();

    // Analize the current pressure
    Lamp::toggleLeds(pressure, securitySensor);
    buzzer->beepBuzzer(pressure);

    // Log the current pressure
    messageManager->printMessage("Pressure: " + String(pressure));
  }
  else
  {
    buzzer->beepState = false;
    buzzer->turnOff();

    redLamp->blinkState = false;
    Lamp::turnOffLamps();
  }

  delay(10);
}
