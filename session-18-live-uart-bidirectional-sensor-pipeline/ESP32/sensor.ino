#include "DHTesp.h"



DHTesp dht;


void sensorTask(void *p) {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  dht.setup(DHT_PIN, DHTesp::DHT22);


  while (true) {

    //read distance
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    float newDistance = pulseIn(ECHO_PIN, HIGH, 30000) / 58.0;

    //read temp and hum
    TempAndHumidity data = dht.getTempAndHumidity();
    float newTemp;
    float newHum;
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      newTemp = g_temperature;
      newHum = g_humidity;

      xSemaphoreGive(dataMutex);
    }
    if (!isnan(data.temperature) && !isnan(data.humidity)) {
      newTemp = data.temperature;
      newHum = data.humidity;
    } else {
      // Serial.println("DHT22 read failed - keeping last values");
    }

    // uptate global and Artuino veriables
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {

      //global veriables
      g_distance = newDistance;
      g_temperature = newTemp;
      g_humidity = newHum;

      xSemaphoreGive(dataMutex);
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}