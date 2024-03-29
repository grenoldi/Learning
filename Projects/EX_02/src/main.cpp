#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define LED 2


TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;


void vTask1(void* pvParameters);
void vTask2(void* pvParameters);


void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle);
  xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task2Handle);

}

void loop() {
  vTaskDelay(3000);
}

void vTask1(void* pvParameters){
  
  while(1){
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void vTask2(void* pvParameters){
  int cont = 0;
  while(1){
    Serial.println("Task 2: "+String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000));
    if(cont>10){
      if(task1Handle != NULL){
        Serial.println("Deletando task 1.");
        vTaskDelete(task1Handle);
        digitalWrite(LED, LOW);
        task1Handle = NULL;
      }
    }
    if(cont>20){
      if(task2Handle != NULL){
        Serial.println("Deletando task 2.");
        vTaskDelete(NULL);
        task2Handle = NULL;
      }
    }
  }
}
