#include <Arduino.h>
#include "DHTesp.h"

DHTesp dht;

const int DHT_PIN = 12;
const int WARNING_LED = 14;
volatile int warning = 0;

void dhtTask(void *param) {

  pinMode(WARNING_LED, OUTPUT);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  Serial.println("=== DHT22 Sensor Ready ===");

  Serial.println("========================================");
  Serial.print("Time");
  Serial.print("\t");
  Serial.print("Temp(C)");
  Serial.print("\t");
  Serial.print("Hum(%)");
  Serial.print("\t");
  Serial.println("Heat");
  Serial.println("========================================");

  while (true) {

    TempAndHumidity data = dht.getTempAndHumidity();
    if (isnan(data.temperature) || isnan(data.humidity)) {
      Serial.println("ERROR: Failed to read DHT22!");
      continue;
    }
    double temp = data.temperature;
    double hum = data.humidity;
    double HI = temp + 0.33 * hum - 4.0;



    Serial.print(millis());
    Serial.print("\t");

    Serial.print(temp, 1);
    Serial.print("\t");

    Serial.print(hum, 1);
    Serial.print("\t");

    Serial.println(HI, 1);
    // Serial.println("Temp: " + String(temp) + " C");
    // Serial.println("Hum:  " + String(hum) + " %");
    // Serial.println("heat:  " + String(HI) + " ");

    if (temp > 30 || hum > 70) {
      Serial.println("WARNING:  " + String(HI) + " on valid.");
      warning = 1;
    }
    else {
      warning = 0;
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void blinkLedTask(void *p) {
  while (true) {
    if (warning) {
      digitalWrite(WARNING_LED, HIGH);
      vTaskDelay(pdMS_TO_TICKS(300));
      digitalWrite(WARNING_LED, LOW);
    }
    else {
      digitalWrite(WARNING_LED, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void setup() {

  Serial.begin(115200);
  xTaskCreate(dhtTask, "DHT", 2048, NULL, 1, NULL);
  xTaskCreate(blinkLedTask, "Blink Led", 1000, NULL, 1, NULL);
}

void loop() {

}
