#define RXD2 4
#define TXD2 5

float joint_angles[18];

void print_angles_table() {
  Serial.println("----- Joint Angles -----");

  for (int i = 0; i < 18; i++) {
    Serial.print("Joint ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(joint_angles[i]);
  }

  Serial.println("------------------------");
}

int parse_angles(String msg) {
  int count = 0;
  int start = 0;

  while (count < 18) {
    int commaIndex = msg.indexOf(',', start);

    String value;

    if (commaIndex == -1) {
      value = msg.substring(start);
    } else {
      value = msg.substring(start, commaIndex);
    }

    value.trim();

    if (value.length() == 0) {
      break;
    }

    joint_angles[count] = value.toFloat();
    count++;

    if (commaIndex == -1) {
      break;
    }

    start = commaIndex + 1;
  }

  return count;
}

void rx_task(void *pvParameters) {
  while (true) {
    if (Serial2.available() > 0) {
      String msg = Serial2.readStringUntil('\n');
      msg.trim();

      Serial.print("RX from RPi: ");
      Serial.println(msg);

      int received_count = parse_angles(msg);

      if (received_count == 18) {
        print_angles_table();

        Serial2.println("{received:18,status:OK}");
        Serial.println("TX to RPi: {received:18,status:OK}");
      } else {
        Serial2.println("{status:ERROR,reason:bad_format}");
        Serial.println("TX to RPi: {status:ERROR,reason:bad_format}");
      }
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void tx_task(void *pvParameters) {
  // String reply;

  while (true) {
    // reply = "hello to RPI from ESP32!!!";
    // Serial2.println(reply);

    // Serial.print("Send to RPi: ");
    // Serial.println(reply);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  xTaskCreate(rx_task, "rx_task", 4096, NULL, 3, NULL);
  xTaskCreate(tx_task, "tx_task", 4096, NULL, 2, NULL);

  Serial.println("ESP32 UART Angles Receiver ready");
}

void loop() {
}