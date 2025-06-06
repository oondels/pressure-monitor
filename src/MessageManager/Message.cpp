#include "MessageManager/Message.h"

Message::Message(unsigned long delay) : delay(delay), lastMessageTime(0) {};
void Message::printMessage(String message) {
    if (millis() - lastMessageTime >= this->delay) {
      Serial.println(message);
      this->lastMessageTime = millis();
    }
  };