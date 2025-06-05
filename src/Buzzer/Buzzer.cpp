#include "Buzzer.h"

Buzzer::Buzzer(int pin) : pin(pin), isOn(false) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void Buzzer::test() {
  turnOn();
  delay(500);
  turnOff();
}

void Buzzer::turnOn() {
  isOn = true;
  digitalWrite(pin, HIGH);
}

void Buzzer::turnOff() {
  isOn = false;
  digitalWrite(pin, LOW);
}