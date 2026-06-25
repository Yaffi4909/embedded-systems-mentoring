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

COMMANDS = {
    "h": ("HOME", HexLeg.HOME),
    "w": ("LIFT", HexLeg.LIFT),
    "s": ("STAND", HexLeg.STAND),
    "a": ("RETRACT", HexLeg.RETRACT),
    "d": ("EXTEND", HexLeg.EXTEND),
}

print("Homework Task 3 - Keyboard Leg Control")
print("Commands:")
print("H = HOME")
print("W = LIFT")
print("S = STAND")
print("A = RETRACT")
print("D = EXTEND")
print("X = EXIT")

try:
    while True:
        key = input("\nEnter command: ").lower().strip()

        if key == "x":
            print("Exit requested.")
            break

        if key not in COMMANDS:
            print("Unknown command. Use H/W/S/A/D/X.")
            continue

        name, position = COMMANDS[key]
        print(f"Moving to {name}: {position}")
        leg0.smooth_to(position, steps=25, delay=0.012)

except KeyboardInterrupt:
    print("\nStopped by user.")

finally:
    print("Returning to HOME...")
    leg0.move_to(HexLeg.HOME)
    time.sleep(1)
    print("Done. Safe to power off.")
