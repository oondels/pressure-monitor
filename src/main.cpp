#include "Lamp/Lamp.h"
#include "Buzzer/Buzzer.h"

#define GREEN_LAMP 26
#define YELLOW_LAMP 25
#define RED_LAMP 27
#define BUZZER_PIN 14

Buzzer *buzzer;
Lamp *greenLamp;
Lamp *yellowLamp;
Lamp *redLamp;

void setup()
{
  Serial.begin(115200);

  Serial.println("Config... \n");
  delay(1000);

  buzzer = new Buzzer(BUZZER_PIN);
  greenLamp = new Lamp(GREEN_LAMP, "Verde");
  yellowLamp = new Lamp(YELLOW_LAMP, "Amarelo");
  redLamp = new Lamp(RED_LAMP, "Vermelho");

  Serial.println("Testing lamps... \n");
  greenLamp->test();
  Serial.println("Testing buzzer... \n");
  buzzer->test();
}

void loop() {}
