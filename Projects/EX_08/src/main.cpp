#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "IRremoteKeyMap.h"

#define LED 2
#define REMOTE 15
#define INT_PIN 4
IRrecv irrecv(REMOTE);

decode_results results;

QueueHandle_t xFila;

TaskHandle_t xTask1Handle;

void vTask1(void* pvParameters);

void trataISR_REMOTE();

void setup() {
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), trataISR_REMOTE, FALLING);

  xFila = xQueueCreate(5, sizeof(int));

  xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask1Handle);

}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void* pvParameters){
  int valor_recebido;
  while(1){
    xQueueReceive(xFila, &valor_recebido, portMAX_DELAY);
    Serial.printf("Valor lido: %x\n", valor_recebido);
    switch(valor_recebido){
      case ON:
        Serial.println("Tecla ON pressionada.");
        break;
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    irrecv.resume();
  }
}

void trataISR_REMOTE(){
  if(irrecv.decode(&results)){
    uint32_t currentMillis = millis();
    while(millis()-currentMillis<200);
    int valor = results.value;
    xQueueSendFromISR(xFila, &valor, NULL);
  }
}