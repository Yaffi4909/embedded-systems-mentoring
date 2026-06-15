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

#include "DHTesp.h"

const int DHT_DATA_PIN = 2;  //rand value

const int TRIG_PIN = 5;        // V
const int ECHO_PIN = 3;        // V
const int LDR_PIN = 6;         // V
const int BLUE_LED_PIN = 4;    // V
const int YELLOW_LED_PIN = 1;  // V
const int DHT_PIN = 2;


SemaphoreHandle_t dataMutex;

BLECharacteristic *pChar;

DHTesp dht;

const char *WIFI_SSID = SECRET_SSID;
const char *WIFI_PASSWORD = SECRET_OPTIONAL_PASS;
const char *POST_SERVER = "http://httpbin.org/post";
const char *GET_SERVER = "http://api.open-meteo.com/v1/forecast?latitude=32&longitude=34&current_weather=true";


//global veriables
volatile float g_temperature = 0.0;  // Celsius
volatile float g_humidity = 0.0;     // Percent
volatile float g_distance = 0.0;     // Centimetres
volatile int g_lightLevel = 0;       // 0-100% (mapped from ADC)
volatile bool g_person = false;      // Person detected flag
volatile bool g_cloud_ok = false;    // Cloud connection status
bool wifiStatus = false;
bool postStatus = false;
int reading_id = 1;


void sensorTask(void *p) {
  while (true) {
    //read LDR
    int raw = analogRead(LDR_PIN);
    int newLightLevel = map(raw, 0, 4095, 0, 100);

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
      Serial.println("DHT22 read failed - keeping last values");
    }

    // uptate global and Artuino veriables
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {

      //global veriables
      g_distance = newDistance;
      g_temperature = newTemp;
      g_humidity = newHum;
      g_lightLevel = newLightLevel;

      //Arduino veriables
      temperature = newTemp;
      humidity = newHum;
      distance = newDistance;
      lightLevel = newLightLevel;


      xSemaphoreGive(dataMutex);
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void presenceTask(void *p) {
  const float PRESENCE_THRESHOLD = 50.0;  // cm
  const int CONFIRM_COUNT = 3;            // 3 consecutive readings
  int counter = 0;
  while (true) {
    bool newPerson = false;
    float distanceCopy = 0;

    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      distanceCopy = g_distance;
      xSemaphoreGive(dataMutex);
    }
    if (distanceCopy > 0 && distanceCopy < PRESENCE_THRESHOLD) {
      counter++;
      if (counter >= CONFIRM_COUNT) {
        newPerson = true;
      }
    } else {
      counter = 0;
      newPerson = false;
    }
    digitalWrite(BLUE_LED_PIN, newPerson ? HIGH : LOW);


    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      g_person = newPerson;
      personDetected = newPerson;

      xSemaphoreGive(dataMutex);
    }
    if (counter > 100000) {
      counter = CONFIRM_COUNT;
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void displayTask(void *parameter) {
  while (true) {
    float tempCopy;
    float humCopy;
    float distCopy;
    int lightCopy;
    bool personCopy;
    bool cloudCopy;
    bool wifiCopy;
    bool postCopy;

    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      tempCopy = g_temperature;
      humCopy = g_humidity;
      distCopy = g_distance;
      lightCopy = g_lightLevel;
      personCopy = g_person;
      cloudCopy = g_cloud_ok;
      wifiCopy = wifiStatus;
      postCopy = postStatus;

      xSemaphoreGive(dataMutex);
    }
    Serial.println("--------------->");

    Serial.print("Temp: ");
    Serial.print(tempCopy);
    Serial.println(" C");

    Serial.print("Hum: ");
    Serial.println(humCopy);

    Serial.print("Distance: ");
    Serial.print(distCopy);
    Serial.println(" cm");

    Serial.print("light level: ");
    Serial.print(lightCopy);
    Serial.println(" %");

    Serial.print("person detected: ");
    Serial.println(personCopy);

    Serial.print("WiFi: ");
    Serial.println(wifiCopy ? "OK" : "FAIL");

    Serial.print("POST: ");
    Serial.println(postCopy ? "OK" : "FAIL");

    Serial.print("Cloud: ");
    Serial.println(cloudCopy ? "Connected" : "Disconnected");
    digitalWrite(YELLOW_LED_PIN, cloudCopy ? HIGH : LOW);


    Serial.println("<---------------");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void bleTask(void *p) {
  while (true) {
    String data;
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      data = "{\"distance\":" + String(g_distance, 1) + ",\"temperature\":" + String(g_temperature, 1) + ",\"humidity\":" + String(g_humidity, 1) + ",\"light\":" + String(g_lightLevel) + "}";
      xSemaphoreGive(dataMutex);
    }
    pChar->setValue(data.c_str());
    pChar->notify();  // Push to connected clients

    Serial.print("BLE Data: ");
    Serial.println(data);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void wifiTask(void *parameter) {
  while (true) {
    wifiStatus = (WiFi.status() == WL_CONNECTED);
    if (wifiStatus) {

      float tempCopy;
      float humCopy;
      float distCopy;
      int lightCopy;
      int idCopy;

      if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
        tempCopy = g_temperature;
        humCopy = g_humidity;
        distCopy = g_distance;
        lightCopy = g_lightLevel;
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
      doc["light_level"] = lightCopy;
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

      // getWeather();

    } else {
      postStatus = false;
      Serial.println("WiFi disconnected");
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void cloudTask(void *p) {
  while (true) {

    ArduinoCloud.update();

    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      g_cloud_ok = ArduinoCloud.connected();
      xSemaphoreGive(dataMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
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

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  while (!Serial) {
    delay(10);
  }
  dataMutex = xSemaphoreCreateMutex();

  Serial.println("\n****************************************");
  Serial.println("********* Mini Project Running *********");
  Serial.println("******** Smart Weather Station! ********");
  Serial.println("****************************************");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  initBLE();

  // Defined in thingProperties.h
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();


  // xTaskCreate(ldrTask, "LDR", 1000, NULL, 1, NULL);
  // xTaskCreate(distanceSensorTask, "distance sensor", 5000, NULL, 1, NULL);
  // xTaskCreate(t_hSensorTask, "temt & hum sensor", 5000, NULL, 1, NULL);
  xTaskCreate(sensorTask, "sensor", 3072, NULL, 1, NULL);
  xTaskCreate(displayTask, "DISPLAY", 2048, NULL, 2, NULL);
  xTaskCreate(presenceTask, "person Detected", 2048, NULL, 2, NULL);
  xTaskCreate(bleTask, "BLE", 4096, NULL, 1, NULL);
  xTaskCreate(wifiTask, "WiFi POST", 8192, NULL, 1, NULL);
  xTaskCreate(cloudTask, "cloud", 8192, NULL, 1, NULL);
}

void loop() {
  // ArduinoCloud.update();
  // Your code here
}
