#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define LED 2
#define REMOTE 15
#define INT_PIN 4
IRrecv irrecv(REMOTE);

SemaphoreHandle_t xSemaforo;
TaskHandle_t xTaskTrataISR_REMOTE_Handle;

void trataISR_REMOTE(void* pvParameters);
void ISR_Callback();

void setup() {
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), ISR_Callback, FALLING);

  xSemaforo = xSemaphoreCreateBinary();
  xTaskCreate(trataISR_REMOTE, "Task Remote", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskTrataISR_REMOTE_Handle);
}

void loop() {
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
  
}

void trataISR_REMOTE(void* pvParameters){
  int contador = 0;
  while(1){
    xSemaphoreTake(xSemaforo, portMAX_DELAY);
    Serial.println("x: " + String(contador++));
  }
}

void ISR_Callback(){
  BaseType_t xHighPriorityTaskWoken = pdTRUE;
  xSemaphoreGiveFromISR(xSemaforo, &xHighPriorityTaskWoken);

  if(xHighPriorityTaskWoken == pdTRUE){
    portYIELD_FROM_ISR();
  }
}