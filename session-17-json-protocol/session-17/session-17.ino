#include "config.h"

float g_joints[18];
uint16_t g_last_seq = 0;


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  for (int i = 0; i < 18; i++) {
    g_joints[i] = 90;
  }

  initServoDriver();
  initLegs();

  delay(500);

  xTaskCreate(
    uartTask,
    "uartTask",
    4096,
    NULL,
    3,
    NULL
  );

  Serial.println("ESP32 UART JSON protocol ready");
}

void loop() {
  delay(1000);
}