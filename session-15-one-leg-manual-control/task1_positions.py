#!/usr/bin/env python3

import time
from adafruit_servokit import ServoKit
from leg import HexLeg


kit = ServoKit(channels=16)

leg0 = HexLeg(
    kit,
    hip_ch=0,
    femur_ch=1,
    tibia_ch=2
)

# New positions for homework task 1
CROUCH = (90, 120, 60)
HIGH_LIFT = (90, 20, 80)
FORWARD_REACH = (150, 60, 120)

NEW_POSITIONS = [
    ("CROUCH", CROUCH),
    ("HIGH_LIFT", HIGH_LIFT),
    ("FORWARD_REACH", FORWARD_REACH),
]


print("Session 15 - Homework Task 1")
print("Testing 3 new leg positions")
print("Press Ctrl+C to stop safely")

try:
    print("Moving to HOME...")
    leg0.move_to(HexLeg.HOME)
    time.sleep(1)

    for name, position in NEW_POSITIONS:
        print(f"Moving to {name}: {position}")
        leg0.smooth_to(position, steps=30, delay=0.015)
        time.sleep(1)

        print("Returning to HOME...")
        leg0.smooth_to(HexLeg.HOME, steps=30, delay=0.015)
        time.sleep(1)

except KeyboardInterrupt:
    print("\nStopped by user.")

finally:
    print("Returning to HOME...")
    leg0.move_to(HexLeg.HOME)
    print("Done. Safe to power off.")
