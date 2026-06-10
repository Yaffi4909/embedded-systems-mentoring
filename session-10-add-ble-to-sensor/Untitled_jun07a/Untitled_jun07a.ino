#include "arduino_secrets.h"
#include "thingProperties.h"

///ble
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pChar;

const int LED_PIN = 2;
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;


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

    String data = "{D:" + String(distance, 1) + "}";

    pChar->setValue(data.c_str());
    pChar->notify();  // Push to connected clients

    Serial.print("BLE Data: ");
    Serial.println(data);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void distanceTask(void *p) {
  while (true) {

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    if (duration > 0) {
      distance = duration / 58.0;
    }
    // Serial.print("Distance in CM is ");
    Serial.print("duration: ");
    Serial.println(duration);

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
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



  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  xTaskCreate(ledTask, "LED", 2000, NULL, 1, NULL);
  xTaskCreate(distanceTask, "distance", 5000, NULL, 1, NULL);
  xTaskCreate(bleTask, "BLE", 4096, NULL, 1, NULL);


  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  // Your code here
}

void onLedChange() {
}
