///ble
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

//WiFi
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Arduino Cloud
#include "arduino_secrets.h"
#include "thingProperties.h"

//wifi global
const char *WIFI_SSID = "ZONG MBB-E5573-24A2";
const char *WIFI_PASSWORD = "90227714";
const char *POST_SERVER = "http://httpbin.org/post";
const char *GET_SERVER = "http://api.open-meteo.com/v1/forecast?latitude=32&longitude=34&current_weather=true";


//ble global
BLECharacteristic *pChar;

//pins
const int LED_PIN = 2;
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;

//global veriables
float g_temp = 0;
float g_hum = 0;
float g_distance = 0;

bool wifiStatus = false;
bool postStatus = false;

float weatherTemp = 0;
float weatherWind = 0;

int reading_id = 1;

SemaphoreHandle_t dataMutex;

void ledTask(void *p) {

  while (true) {
    digitalWrite(LED_PIN, led ? HIGH : LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void bleTask(void *p) {
  while (true) {
    String data;
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      data = "{D:" + String(g_distance, 1) + ", T:" + String(g_temp, 1) + ", H:" + String(g_hum, 1) + ", }";
      xSemaphoreGive(dataMutex);
    }
    pChar->setValue(data.c_str());
    pChar->notify();  // Push to connected clients

    Serial.print("BLE Data: ");
    Serial.println(data);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void distanceSensorTask(void *p) {
  while (true) {

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    float newDistance = 0;
    if (duration > 0) {
      newDistance = duration / 58.0;

      if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
        g_distance = newDistance;
        distance = newDistance;  // משתנה של Arduino Cloud

        xSemaphoreGive(dataMutex);
      }
    }

    // Serial.print("duration: ");
    // Serial.print(duration);
    // Serial.print(" ---> ");
    // Serial.print(newDistance);
    // Serial.println(" CM.");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void t_hSensorTask(void *parameter) {
  while (true) {
    float newTemp = 25.1 + random(-2, 3);
    float newHum = 60.0 + random(-5, 6);


    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      g_temp = newTemp;
      g_hum = newHum;

      temp = newTemp;
      hum = newHum;

      xSemaphoreGive(dataMutex);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void wifiTask(void *parameter) {
  while (true) {
    wifiStatus = (WiFi.status() == WL_CONNECTED);
    if (wifiStatus) {

      float tempCopy;
      float humCopy;
      float distCopy;
      int idCopy;

      if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
        tempCopy = g_temp;
        humCopy = g_hum;
        distCopy = g_distance;
        idCopy = reading_id++;

        xSemaphoreGive(dataMutex);
      }

      HTTPClient http;

      http.begin(POST_SERVER);
      http.addHeader("Content-Type", "application/json");

      //build json
      StaticJsonDocument<250> doc;

      doc["reading_id"] = idCopy;
      doc["temperature"] = tempCopy;
      doc["humidity"] = humCopy;
      doc["distance"] = distCopy;
      doc["device"] = "RoboGreeno-ESP32";

      String body;
      serializeJson(doc, body);

      int code = http.POST(body);

      if (code == 200) {
        postStatus = true;
        Serial.println("POST sent OK:");
        Serial.println(body);
      } else {
        postStatus = false;
        Serial.print("POST failed. Code: ");
        Serial.println(code);
      }

      http.end();

      getWeather();

    } else {
      postStatus = false;
      Serial.println("WiFi disconnected");
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void displayTask(void *parameter) {
  while (true) {
    Serial.println("---------------");

    Serial.print("Temp: ");
    Serial.println(g_temp);

    Serial.print("Hum: ");
    Serial.println(g_hum);

    Serial.print("Distance: ");
    Serial.println(g_distance);

    Serial.print("WiFi: ");
    Serial.println(wifiStatus ? "OK" : "FAIL");

    Serial.print("POST: ");
    Serial.println(postStatus ? "OK" : "FAIL");

    Serial.print("Weather temp: ");
    Serial.println(weatherTemp);

    Serial.print("Weather wind: ");
    Serial.println(weatherWind);

    Serial.println("---------------");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("WiFi Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
}

void initBLE() {

  //Initialize BLE device and server
  BLEDevice::init("RoboGreeno-ESP32");  // Device name
  BLEServer *pServer = BLEDevice::createServer();

  // Create a Service and Characteristic
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pChar = pService->createCharacteristic(
    CHAR_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pChar->setValue("Temp:25.0 Hum:60.0");  // Initial value
  pService->start();

  // Start advertising
  BLEAdvertising *pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("BLE advertising started!");
}

void getWeather() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("GET skipped - WiFi disconnected");
    return;
  }

  HTTPClient http;
  http.begin(GET_SERVER);

  int code = http.GET();

  if (code == 200) {
    String payload = http.getString();

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      weatherTemp = doc["current_weather"]["temperature"];
      weatherWind = doc["current_weather"]["windspeed"];

      Serial.print("Current weather: ");
      Serial.print(weatherTemp);
      Serial.print(" C, wind: ");
      Serial.print(weatherWind);
      Serial.println(" km/h");
    } else {
      Serial.println("GET JSON parse failed");
    }
  } else {
    Serial.print("GET failed. Code: ");
    Serial.println(code);
  }

  http.end();
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);

  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  while (!Serial) {
    delay(10);
  }

  // Serial.println("--- SYSTEM START ---");
  Serial.println("--- SESSION 10 BLE ---");

  dataMutex = xSemaphoreCreateMutex();

  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  initBLE();

  //connect wifi
  // connectToWiFi();

  //create tasks
  xTaskCreate(ledTask, "LED", 2000, NULL, 1, NULL);
  xTaskCreate(distanceSensorTask, "distance sensor", 5000, NULL, 1, NULL);
  xTaskCreate(t_hSensorTask, "temt & hum sensor", 5000, NULL, 1, NULL);
  xTaskCreate(bleTask, "BLE", 4096, NULL, 1, NULL);
  xTaskCreate(wifiTask, "WIFI", 8192, NULL, 1, NULL);
  xTaskCreate(displayTask, "DISPLAY", 4096, NULL, 2, NULL);
}

void loop() {
  ArduinoCloud.update();
  // Your code here
}

void onLedChange() {
}
