#include <Arduino.h>

#define LED 2
#define DELAY_TIME 1000

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(DELAY_TIME);
  digitalWrite(LED, LOW);
  delay(DELAY_TIME);
  Serial.println("HELLO!");
}