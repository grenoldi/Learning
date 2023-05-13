#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#define LED 2
#define TASK1_FLAG ( 1 << 0 )
#define TASK2_FLAG ( 1 << 1 )

TaskHandle_t xTask1Handle, xTask2Handle;
TimerHandle_t xTimer;
EventGroupHandle_t xEvent;

void vTask1(void* pvParameters);
void vTask2(void* pvParameters);
void callbackTimer(TimerHandle_t xTimer);

long tempo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  xEvent = xEventGroupCreate();
  xTimer = xTimerCreate("TIMER 1", pdMS_TO_TICKS(1000), pdTRUE, 0, callbackTimer);
  xTaskCreate(vTask1, "TASK 1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1Handle);
  xTaskCreate(vTask2, "TASK 2", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask2Handle);

  xTimerStart(xTimer, 0);
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(500));
}

void vTask1(void* pvParameters){
  EventBits_t xBits;
  while (1){
    xBits = xEventGroupWaitBits(xEvent, TASK1_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println("Task 1 saiu do estado de bloqueio.");
  }
}

void vTask2(void* pvParameters){
  EventBits_t xBits;
  while (1){
    xBits = xEventGroupWaitBits(xEvent, TASK2_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println("Task 2 saiu do estado de bloqueio.");
  }
}

void callbackTimer(TimerHandle_t xTimer){
  tempo++;
  if(tempo == 5){
    xEventGroupSetBits(xEvent, TASK1_FLAG);
  }

  else if(tempo == 10){
    xEventGroupSetBits(xEvent, TASK2_FLAG);
  }
  else if(tempo == 15){
    tempo = 0;
    xEventGroupSetBits(xEvent, TASK1_FLAG | TASK2_FLAG);
  }
  
}