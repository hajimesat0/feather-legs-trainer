#include <Arduino.h>

const int LED_PIN = 33;

void setup() {
  printf("setup funtion\n");

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  printf("loop function\n");

digitalWrite(LED_PIN, HIGH);
delay(500);
digitalWrite(LED_PIN, LOW);
delay(500);

}

