#include "config.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void initServoDriver() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("I2C scanner started");

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found I2C device at 0x");
      Serial.println(address, HEX);
    }
  }

  Serial.println("Scan done");
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
}

int angleToPulse(int angle) {
  angle = constrain(angle, 0, 180);

  int pulseUs = map(angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);

  int pulseCount = (int)((pulseUs * 4096.0) / 20000.0);

  return pulseCount;
}

void setServoAngle(int channel, int angle) {
  int pulse = angleToPulse(angle);

  pwm.setPWM(channel, 0, pulse);

  // Serial.print("Channel ");
  // Serial.print(channel);
  // Serial.print(" angle=");
  // Serial.print(angle);
  // Serial.print(" pulse=");
  // Serial.println(pulse);
}

void servoTask(void *p) {
  while (true) {
    updateLegs();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}