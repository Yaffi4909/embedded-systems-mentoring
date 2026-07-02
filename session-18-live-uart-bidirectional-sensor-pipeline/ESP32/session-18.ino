#include "config.h"

//global veriables
volatile float g_temperature = 0.0;  // Celsius
volatile float g_humidity = 0.0;     // Percent
volatile float g_distance = 0.0;     // Centimetres

float g_joints[18];
uint16_t g_last_seq = 0;
volatile int g_status_seq = 0;

SemaphoreHandle_t dataMutex;


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  for (int i = 0; i < 18; i++) {
    g_joints[i] = 90;
  }
  delay(10000);

  dataMutex = xSemaphoreCreateMutex();

  initServoDriver();
  initLegs();

  pinMode(ALERT_LED_PIN, OUTPUT);
  digitalWrite(ALERT_LED_PIN, LOW);

  delay(500);

  xTaskCreate(uartTask, "uartTask", 4096, NULL, 3, NULL);
  xTaskCreate(sensorTask, "sensor", 3072, NULL, 1, NULL);
  xTaskCreate(statusTask, "status", 3072, NULL, 1, NULL);
  xTaskCreate(servoTask, "servo", 3072, NULL, 1, NULL);

  Serial.println("ESP32 UART JSON protocol ready");
}

void loop() {
}