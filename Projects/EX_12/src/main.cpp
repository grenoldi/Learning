#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define LED 2

SemaphoreHandle_t mutex;

TaskHandle_t xTask1Handle, xTask2Handle;

void vTask1(void* pvParameters);

void vTask2(void* pvParameters);

void enviaInformacao(int i);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  mutex = xSemaphoreCreateMutex();
  xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1Handle);
  xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE + 1024, NULL, 4, &xTask2Handle);
}

void loop() {
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
  
}

void vTask1(void* pvParameters){
  while(1){
    xSemaphoreTake(mutex, portMAX_DELAY);
    enviaInformacao(1);
    delay(3000);
    xSemaphoreGive(mutex);
    vTaskDelay(10);
  }
}

void vTask2(void* pvParameters){
  while(1){
    xSemaphoreTake(mutex, portMAX_DELAY);
    enviaInformacao(2);
    delay(500);
    xSemaphoreGive(mutex);
    vTaskDelay(10); // beware of this delay, its removal can cause task 1 starvation
  }
}

void enviaInformacao(int i){
  Serial.println("Enviando informacao da task: " + String(i));
}