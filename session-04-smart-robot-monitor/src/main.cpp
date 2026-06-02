#include <string>
#include <Arduino.h>
#include "DHTesp.h"

// Measures Distance - ultara sonic
int TRIG = 33;
int ECHO = 34;

// int UST_DOWN = 20;
// int USE_DOWN = 21;

// int UST_RIGHT = 38;
// int USE_RIGHT = 37;

// int UST_LEFT = 8;
// int USE_LEFT = 18;

// int trigs[] = {UST_UP, UST_DOWN, UST_RIGHT, UST_LEFT};
// int echos[] = {USE_UP, USE_DOWN, USE_RIGHT, USE_LEFT};
// String sides[] = {"UP", "DOWN", "RIGHT", "LEFT"};
// bool valid_dis[] = {1, 1, 1, 1};

// Temperature and Humidity
int DHT = 13;
DHTesp dhtSensor;

// int DHT_NC = 14;

// led
int LED = 23;

void distanceTask(void *p)
{
  // int index_task = *((int *)p);
  while (true)
  {

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    // Read the result:
    int duration = pulseIn(ECHO, HIGH);
    if (duration / 58 < 20)
    {
      // valid_dis[index_task] = 0;
      Serial.print("Distance in CM: ");
      Serial.println(duration / 58);
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void dhtTask(void *p)
{
  while (true)
  {
    float humidity = dhtSensor.getHumidity();
    float temperature = dhtSensor.getTemperature();

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// void checkValidDistanceTask(void *p)
// {
//   while (true)
//   {
//     if (!(valid_dis[0] && valid_dis[1] && valid_dis[2] && valid_dis[3]))
//     {
//       digitalWrite(LED, HIGH);
//     }
//     else
//     {
//       digitalWrite(LED, LOW);
//     }
//     vTaskDelay(100 / portTICK_PERIOD_MS);
//   }
// }

void setup()
{

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S3!");

  pinMode(LED, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  dhtSensor.setup(DHT, DHTesp::DHT22);

  xTaskCreate(distanceTask, "distance", 3000, NULL, 1, NULL);
  xTaskCreate(dhtTask, "dht", 3000, NULL, 1, NULL);
}

void loop()
{
}
