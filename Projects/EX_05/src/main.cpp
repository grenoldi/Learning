#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define LED1 2
#define LED2 14

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;


void vTaskBlink(void* pvParameters);
void vTask2(void* pvParameters);

int valor = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  xTaskCreatePinnedToCore(vTaskBlink, "TASK1", configMINIMAL_STACK_SIZE, (void*) LED1, 1, &task1Handle, APP_CPU_NUM);
  xTaskCreatePinnedToCore(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, (void*) valor, 2, &task2Handle, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(vTaskBlink, "TASK3", configMINIMAL_STACK_SIZE, (void*) LED2, 2, &task3Handle, APP_CPU_NUM);

}

void loop() {
  vTaskDelay(3000);
}

void vTaskBlink(void* pvParameters){
  int pin = (int)pvParameters;
  while(1){
    digitalWrite(pin, !digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void vTask2(void* pvParameters){
  int cont = (int)pvParameters;
  while(1){
    Serial.println("Task 2: "+String(cont++));

    vTaskDelay(pdMS_TO_TICKS(1000));
    if(cont==10){
      Serial.println("Pausando task 3.");
      vTaskSuspend(task3Handle);
      digitalWrite(LED2, LOW);
    }
    if(cont==20){
      Serial.println("Pausando task 1.");
      vTaskSuspend(task1Handle);
      digitalWrite(LED1, LOW);
    }
    /*if(cont==30){
      Serial.println("Retomando task 1.");
      vTaskResume(task1Handle);
    }
    if(cont==40){
      Serial.println("Retomando task 3.");
      vTaskResume(task3Handle);
      cont = 0;
    }*/
  }
}
