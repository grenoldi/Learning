#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED 2
#define REMOTE 15
#define INT_PIN 4
IRrecv irrecv(REMOTE);

TaskHandle_t xTaskTrataRemoteHandle;

void vTaskTrataRemote(void* pvParameters);
void callbackRemote();

void setup() {
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(INT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(INT_PIN), callbackRemote, FALLING);

  xTaskCreate(vTaskTrataRemote, "TASK REMOTE", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskTrataRemoteHandle);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTaskTrataRemote(void* pvParameters){
  uint32_t quantidade = 0;
  while(1){
    quantidade = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Tratando interrupcao do controle: " + String(quantidade));
    digitalWrite(LED, !digitalRead(LED));
    delay(3000);
  }
}

void callbackRemote(void){
  vTaskNotifyGiveFromISR(xTaskTrataRemoteHandle, 0);
  long current_micros=micros();
  //while(micros()-current_micros < 130000);
}