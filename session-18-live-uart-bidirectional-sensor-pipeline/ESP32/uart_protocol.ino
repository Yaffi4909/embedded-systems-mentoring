#include <ArduinoJson.h>
#include "config.h"

void sendACK(uint16_t seq, int applied) {
  StaticJsonDocument<128> doc;

  doc["type"] = "ACK";
  doc["seq"] = seq;
  doc["ts"] = millis();
  doc["data"]["status"] = "OK";
  doc["data"]["applied"] = applied;

  String out;
  serializeJson(doc, out);
  Serial2.println(out);

  Serial.print("send ACK : seq = ");
  Serial.println(seq);
}

void sendERR(uint16_t seq, const char* code, const char* msg) {
  StaticJsonDocument<192> doc;

  doc["type"] = "ERR";
  doc["seq"] = seq;
  doc["ts"] = millis();
  doc["data"]["code"] = code;
  doc["data"]["msg"] = msg;

  String out;
  serializeJson(doc, out);
  Serial2.println(out);

  Serial.print("send ERR : seq = ");
  Serial.print(seq);
  Serial.print(", ");
  Serial.println(msg);
}

void parseFrame(String raw) {
  StaticJsonDocument<512> doc;

  DeserializationError err = deserializeJson(doc, raw);

  if (err) {
    sendERR(0, "BAD_JSON", err.c_str());
    return;
  }

  const char* type = doc["type"];
  uint16_t seq = doc["seq"] | 0;

  if (type == nullptr) {
    sendERR(seq, "NO_TYPE", "Missing type field");
    return;
  }

  if (strcmp(type, "CMD") == 0) {
    JsonArray joints = doc["data"]["joints"];
    int speed = doc["data"]["speed"] | 25;

    if (joints.size() != 18) {
      sendERR(seq, "BAD_SIZE", "Need exactly 18 joints");
      return;
    }

    for (int i = 0; i < 18; i++) {
      float angle = joints[i];

      if (angle < 0 || angle > 180) {
        sendERR(seq, "BAD_ANGLE", "Angle must be 0-180");
        return;
      }

      g_joints[i] = angle;
    }

    g_last_seq = seq;

    Serial.print("CMD received seq=");
    Serial.print(seq);
    Serial.print(" speed=");
    Serial.println(speed);


    // updateLegs();

    sendACK(seq, 18);
    return;
  }
  else if (strcmp(type, "ALERT") == 0) {
    Serial.println("ALERT received from RPi");

    blinkAlertLed();

    StaticJsonDocument<128> ack;
    ack["type"] = "ACK";
    ack["seq"] = doc["seq"];
    ack["ts"] = millis();
    ack["data"]["status"] = "ALERT_OK";

    String out;
    serializeJson(ack, out);
    Serial2.println(out);
  }

  sendERR(seq, "UNKNOWN_TYPE", "Unsupported frame type");
}

void uartTask(void* p) {
  while (true) {
    if (Serial2.available()) {
      String raw = Serial2.readStringUntil('\n');
      raw.trim();

      if (raw.length() > 0) {
        parseFrame(raw);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void statusTask(void* p) {
  while (true) {
    g_status_seq++;
    StaticJsonDocument<256> doc;

    doc["type"] = "STATUS";
    doc["seq"] = g_status_seq;
    doc["ts"] = millis();
    doc["data"]["bat"] = 85;  //palce holder

    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {

      doc["data"]["temp"] = g_temperature;
      doc["data"]["dist"] = g_distance;
      doc["data"]["hum"] = g_humidity;
      Serial.print("STATUS: ");
      Serial.print(g_temperature);
      Serial.print(", ");
      Serial.print(g_distance);
      Serial.print(", ");
      Serial.println(g_humidity);


      xSemaphoreGive(dataMutex);
    }

    String out;
    serializeJson(doc, out);

    Serial2.println(out);

    Serial.println("send STATUS");
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void blinkAlertLed() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ALERT_LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(200));

    digitalWrite(ALERT_LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}
