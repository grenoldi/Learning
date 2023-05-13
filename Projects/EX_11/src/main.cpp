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

SemaphoreHandle_t xSemaforoContador;
TaskHandle_t xTaskTrataISR_REMOTE_Handle;

void trataISR_REMOTE(void* pvParameters);
void ISR_Callback();

void setup() {
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), ISR_Callback, FALLING);

  xSemaforoContador = xSemaphoreCreateCounting(255,0);
  xTaskCreate(trataISR_REMOTE, "Task Remote", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskTrataISR_REMOTE_Handle);
}

void loop() {
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
  
}

void trataISR_REMOTE(void* pvParameters){
  while(1){
    xSemaphoreTake(xSemaforoContador, portMAX_DELAY);
    Serial.print("Tratando a ISR do controle remoto: ");

    UBaseType_t x = uxSemaphoreGetCount(xSemaforoContador);

    Serial.println(x);
    delay(1000);
  }
}

void ISR_Callback(){
  long current_micros = micros();
  while(micros() - current_micros < 100000);
  BaseType_t xHighPriorityTaskWoken = pdTRUE;
  xSemaphoreGiveFromISR(xSemaforoContador, &xHighPriorityTaskWoken);

  if(xHighPriorityTaskWoken == pdTRUE){
    portYIELD_FROM_ISR();
  }
}