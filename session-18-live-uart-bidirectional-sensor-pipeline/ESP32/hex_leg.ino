class HexLeg {
private:
  int hipChannel;
  int femurChannel;
  int tibiaChannel;

  float hipAngle;
  float femurAngle;
  float tibiaAngle;

public:
  HexLeg() {
    hipChannel = -1;
    femurChannel = -1;
    tibiaChannel = -1;

    hipAngle = 90;
    femurAngle = 90;
    tibiaAngle = 90;
  }

  HexLeg(int hip, int femur, int tibia) {
    init(hip, femur, tibia);
  }

  void init(int hip, int femur, int tibia) {
    hipChannel = hip;
    femurChannel = femur;
    tibiaChannel = tibia;

    hipAngle = 90;
    femurAngle = 90;
    tibiaAngle = 90;
  }

  void moveTo(float hip, float femur, float tibia) {
    hipAngle = constrain(hip, 0, 180);
    femurAngle = constrain(femur, 0, 180);
    tibiaAngle = constrain(tibia, 0, 180);

    setServoAngle(hipChannel, hipAngle);
    delay(20);

    setServoAngle(femurChannel, femurAngle);
    delay(20);

    setServoAngle(tibiaChannel, tibiaAngle);
    delay(20);
  }

  void home() {
    moveTo(90, 90, 90);
  }

  void stand() {
    moveTo(90, 60, 120);
  }

  void setHip(float angle) {
    hipAngle = constrain(angle, 0, 180);
    setServoAngle(hipChannel, hipAngle);
  }

  void setFemur(float angle) {
    femurAngle = constrain(angle, 0, 180);
    setServoAngle(femurChannel, femurAngle);
  }

  void setTibia(float angle) {
    tibiaAngle = constrain(angle, 0, 180);
    setServoAngle(tibiaChannel, tibiaAngle);
  }

  void moveByOffset(float hipOffset, float femurOffset, float tibiaOffset) {
    moveTo(
      hipAngle + hipOffset,
      femurAngle + femurOffset,
      tibiaAngle + tibiaOffset
    );
  }

  void printState(int legIndex) {
    Serial.print("Leg ");
    Serial.print(legIndex);
    Serial.print(": hip=");
    Serial.print(hipAngle);
    Serial.print(" femur=");
    Serial.print(femurAngle);
    Serial.print(" tibia=");
    Serial.println(tibiaAngle);
  }
};