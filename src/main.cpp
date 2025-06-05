#include "Lamp/Lamp.h"
#include "Buzzer/Buzzer.h"
#include "PressureSensor/PressureSensor.h"

#define GREEN_LAMP 26
#define YELLOW_LAMP 25
#define RED_LAMP 27
#define BUZZER_PIN 14

#define SENSOR_PIN 34
float max_pressure = 8.0;
float min_pressure = 2.0;

PressureSensor *pressureSensor;
Buzzer *buzzer;
Lamp *greenLamp;
Lamp *yellowLamp;
Lamp *redLamp;

// Variáveis para controle de tempo de log
// Mover para arquivo independente
unsigned long lastLogTime = 0;
unsigned long logInterval = 1000;

void setup()
{
  Serial.begin(115200);

  Serial.println("Initializing...");
  delay(1000);

  pressureSensor = new PressureSensor(SENSOR_PIN, max_pressure, min_pressure);
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
  int samples = 20;
  pressureSensor->readSensorSignal(samples);
  float pressure = pressureSensor->getPressure();

  // Analize the current pressure
  Lamp::toggleLeds(pressure);
  buzzer->beepBuzzer(pressure);

  // Log the current pressure
  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime >= logInterval)
  {
    Serial.println("Pressure: ");
    Serial.print(pressure);
    Serial.println();
    Serial.println();

    lastLogTime = currentMillis;
  }

  delay(10);
}
