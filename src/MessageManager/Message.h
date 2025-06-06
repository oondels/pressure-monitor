#pragma once
#include <Arduino.h>

class PressureSensor;

class Message
{
public:
  Message(unsigned long delay);
  unsigned long delay;
  unsigned long lastMessageTime;
  void printMessage(String message);
  //* Future: Upgrade to send data to server...
};