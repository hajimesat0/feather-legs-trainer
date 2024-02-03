#include <Arduino.h>

const int LED_PIN = 33;
const int BUTTON_PIN = 35;

void setup() {
  printf("setup funtion\n");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  printf("loop function\n");

  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

}

