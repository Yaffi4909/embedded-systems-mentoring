HexLeg legs[6];

void initLegs() {
  Serial.println("init legs");

  legs[0].init(0, 1, 2);
  legs[1].init(3, 4, 5);
  legs[2].init(6, 7, 8);
  legs[3].init(9, 10, 11);
  legs[4].init(12, 13, 14);
  legs[5].init(15, 16, 17);

  homeAllLegs();
}

void homeAllLegs() {
  for (int i = 0; i < 6; i++) {
    legs[i].home();
    delay(50);
  }
}

void standAllLegs() {
  for (int i = 0; i < 6; i++) {
    legs[i].stand();
    delay(50);
  }
}


void updateLegs() {
  Serial.println("update legs");

  for (int leg = 0; leg < 6; leg++) {
    int base = leg * 3;

    legs[leg].moveTo(
      g_joints[base],
      g_joints[base + 1],
      g_joints[base + 2]);
  }
}

void printAllLegs() {
  Serial.println("print all legs");

  for (int i = 0; i < 6; i++) {
    legs[i].printState(i);
  }
}