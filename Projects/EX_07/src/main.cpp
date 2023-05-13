#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos//queue.h"

#define LED 2

QueueHandle_t xFila;

TaskHandle_t xTask1Handle, xTask2Handle;

void vTask1(void* pvParameters);
void vTask2(void* pvParameters);

void setup() {
  BaseType_t xTaskCreated;
  Serial.begin(9600);
 
  pinMode(LED, OUTPUT);
 
  vTaskDelay(1000);

  xFila = xQueueCreate(5, sizeof(int));

  if(xFila == NULL){
    Serial.println("Nao foi possivel criar a fila.");
    while(1);
  }

  xTaskCreated = xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask1Handle);

  if(xTaskCreated == pdFAIL){
    Serial.println("Nao foi possivel criar a Task 1.");
    while(1);
  }

  xTaskCreated = xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask2Handle);

  if(xTaskCreated == pdFAIL){
    Serial.println("Nao foi possivel criar a Task 2.");
    while(1);
  }

}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void* parameters){
  int count = 0;
  while (1){
    if(count < 10){
      Serial.println("Task 1: Sending data to queue - count = "+String(count));
      xQueueSend(xFila, &count, portMAX_DELAY);
      count++;
    }
    else{
      count = 0;
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void vTask2(void* parameters){
  int valorRecebido = 0;
  while (1){
    if(xQueueReceive(xFila, &valorRecebido, pdMS_TO_TICKS(1000))==pdTRUE){
      Serial.println("Task 2: Valor recebido = "+String(valorRecebido));
    }
    else{
      Serial.println("Task 2: TIMEOUT.");
    }
  }
}