#define RXD2 4
#define TXD2 5

void rx_task(void *pvParameters) {
  while (true) {
    if (Serial2.available() > 0) {
      String msg = Serial2.readStringUntil('\n');
      msg.trim();

      Serial.print("RX from RPi: ");
      Serial.println(msg);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void tx_task(void *pvParameters) {
  String reply;

  while (true) {
    reply = "hello to RPI from ESP32!!!";
    Serial2.println(reply);

    Serial.print("Send to RPi: ");
    Serial.println(reply);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  xTaskCreate(rx_task, "rx_task", 4096, NULL, 3, NULL);
  xTaskCreate(tx_task, "tx_task", 4096, NULL, 2, NULL);

  Serial.println("ESP32 UART FreeRTOS ready");
}

void loop() {
}
