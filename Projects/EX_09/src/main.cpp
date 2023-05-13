#include <Arduino.h>


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>


#define LED 2

TaskHandle_t xTaskADCHandle;

SemaphoreHandle_t xSemaphore;

void vTaskADC(void* pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  xSemaphore = xSemaphoreCreateBinary();

  if(xSemaphore == NULL){
    Serial.println("Nao foi possivel criar o semaforo");
    while(1);
  }

  xTaskCreate(vTaskADC, "Task ADC", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskADCHandle);
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  Serial.println("Valor do LED: " + String(digitalRead(LED)));
  vTaskDelay(pdMS_TO_TICKS(3000));
  xSemaphoreGive(xSemaphore);
}

void vTaskADC(void* pvParameters){
  int adcValue;
  while(1){
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    adcValue = analogRead(34);
    Serial.println("Valor do ADC: " + String(adcValue));
  }
}