#include <Arduino.h>

const int buttonPin = 33;
const int ledPin = 23;

QueueHandle_t queue;

void buttonTask(void *pvParameters)
{

  while (1)
  {
    int buttonState = digitalRead(buttonPin);
    Serial.print("send: ");
    Serial.println(buttonState);
    xQueueSend(queue, &buttonState, portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS); // Check button state every 100ms
  }
}

void ledTask(void *pvParameters)
{
  int buttonState;

  while (true)
  {
    if (xQueueReceive(queue, &buttonState, portMAX_DELAY) == pdPASS)
    {
      Serial.print("get: ");
      Serial.println(buttonState);
      if (buttonState == HIGH)
      {
        digitalWrite(ledPin, HIGH); // Turn on LED
      }
      else
      {
        digitalWrite(ledPin, LOW); // Turn off LED
      }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS); // Update LED state every 100ms
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting FreeRTOS Queue Example");

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  queue = xQueueCreate(10, sizeof(int));

  xTaskCreate(buttonTask, "Button Task", 1000, NULL, 1, NULL);
  xTaskCreate(ledTask, "LED Task", 1000, NULL, 1, NULL);
}

void loop()
{
}
