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

STEP_CYCLE = [
    ("STAND", HexLeg.STAND),
    ("LIFT", HexLeg.LIFT),
    ("EXTEND", HexLeg.EXTEND),
    ("STAND", HexLeg.STAND),
    ("RETRACT", HexLeg.RETRACT),
    ("STAND", HexLeg.STAND),
]

print("Session 15 - One Leg Step Demo")
print("Press Ctrl+C to stop safely")

try:
    print("Moving to HOME...")
    leg0.move_to(HexLeg.HOME)
    time.sleep(1)

    for cycle in range(1, 11):
        print(f"--- Step cycle #{cycle} ---")

        for name, position in STEP_CYCLE:
            print(f"Moving to {name}: {position}")
            leg0.smooth_to(position, steps=25, delay=0.012)

        time.sleep(0.3)

except KeyboardInterrupt:
    print("\nStopped by user.")

finally:
    print("Returning to HOME...")
    leg0.move_to(HexLeg.HOME)
    print("Done. Safe to power off.")
