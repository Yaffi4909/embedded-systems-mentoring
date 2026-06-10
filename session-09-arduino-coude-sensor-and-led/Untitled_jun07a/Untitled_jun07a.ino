#include "arduino_secrets.h"
#include "thingProperties.h"

const int LED_PIN = 2;
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;

int valid = 0;


void ledTask(void *p) {

  while (true) {
    digitalWrite(LED_PIN, led ? HIGH : LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void distanceTask(void *p) {
  while (true) {

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);

    distance = duration / 58;
    // Serial.print("Distance in CM is ");
    Serial.print("duration: ");
    Serial.println(duration);

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);

  while (!Serial) {  // המתנה חיונית לחיבור USB
    delay(10);
  }

  delay(1500);
  
  Serial.println("");
  Serial.println("--- SYSTEM START ---");
  led = false;
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  xTaskCreate(ledTask, "LED", 2000, NULL, 1, NULL);
  xTaskCreate(distanceTask, "distance", 5000, NULL, 1, NULL);


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
