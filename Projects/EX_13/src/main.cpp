#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#define LED1 2
#define LED2 14
#define REMOTE 15
#define INT_PIN 4
IRrecv irrecv(REMOTE);

TaskHandle_t xTask1Handle;
TaskHandle_t xTimer1, xTimer2;

void vTask1(void* pvParameters);
void callbackTimer1(TimerHandle_t xTimer);
void callbackTimer2(TimerHandle_t xTimer);

void setup() {
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(INT_PIN, INPUT_PULLUP);
  
  xTimer1 = xTimerCreate("TIMER 1", pdMS_TO_TICKS(500), pdTRUE, 0, callbackTimer1);
  xTimer2 = xTimerCreate("TIMER 2", pdMS_TO_TICKS(10000), pdFALSE, 0, callbackTimer2);

  xTaskCreate(vTask1, "TASK 1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1Handle);
  xTimerStart(xTimer1, 0);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void* pvParameters){
  while(1){
    if(digitalRead(INT_PIN) == LOW && xTimerIsTimerActive(xTimer2) == pdFALSE){
      delay(150);
      digitalWrite(LED2, HIGH);
      xTimerStart(xTimer2, 0);
      xTimerStop(xTimer1, 0);
      Serial.println("Iniciando timer 2.");
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void callbackTimer1(TimerHandle_t xTimer){
  digitalWrite(LED1, !digitalRead(LED1));
}

void callbackTimer2(TimerHandle_t xTimer){
  digitalWrite(LED2, LOW);
  xTimerStart(xTimer1, 0);
}
