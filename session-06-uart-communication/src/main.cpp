#include <Arduino.h>

#define LED_PIN 13
#define TX 16
#define RX 17

void buttonTask(void *pvParameters)
{
  Serial.println("Button Task is running...");
  while (1)
  {
    if (Serial.available() > 0)
    {                          
      char cmd = Serial.read(); 

      if (cmd == '1')
      {
        digitalWrite(LED_PIN, HIGH); 
        Serial.println("LED ON");
      }
      else if (cmd == '0')
      {
        digitalWrite(LED_PIN, LOW); 
        Serial.println("LED OFF");
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); 
  }
}

void rx_task(void *pvParameters)
{
  Serial.println("RX Task is running...");
  while (1)
  {
    if (digitalRead(RX) == LOW)
    {
      Serial.println("RX LOW");
    }
    else
    {
      Serial.println("RX HIGH");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
  }
}

void tx_task(void *pvParameters)
{
  Serial.println("TX Task is running...");
  while (1)
  {
    digitalWrite(TX, HIGH); 
    Serial.println("TX HIGH");
    vTaskDelay(1000 / portTICK_PERIOD_MS); 

    digitalWrite(TX, LOW); 
    Serial.println("TX LOW");
    vTaskDelay(1000 / portTICK_PERIOD_MS); 
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello, Yaffi!");

  pinMode(LED_PIN, OUTPUT);
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);

  digitalWrite(TX, LOW);
  digitalWrite(RX, HIGH);


  xTaskCreate(buttonTask, "Button Task", 1000, NULL, 1, NULL);
  xTaskCreate(tx_task, "TX Task", 1000, NULL, 1, NULL);
  xTaskCreate(rx_task, "RX Task", 1000, NULL, 1, NULL);
}

void loop()
{
}
