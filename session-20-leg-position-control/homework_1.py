import time
import json
from adafruit_servokit import ServoKit
from leg_controller import LegController
from trajectory import STEP_CYCLE


kit = ServoKit(channels=16)

leg = LegController(
    kit=kit,
    hip_ch=0,
    fem_ch=1,
    tib_ch=2
)

try:
    with open("offsets.json", "r") as f:
        off = json.load(f)

    leg.HIP_OFF = off["hip"]
    leg.FEMUR_OFF = off["femur"]
    leg.TIBIA_OFF = off["tibia"]

    print("Offsets loaded:", off)

except FileNotFoundError:
    print("offsets.json not found, using default offsets")


print("Session 20: IK Position Control")

leg.go_home()
time.sleep(1)

for cycle in range(3):
    print(f"--- Cycle {cycle + 1} ---")

    for pos in STEP_CYCLE:
        print("Moving to:", pos)
        leg.move_foot_to(*pos, steps=25)
        time.sleep(0.15)

print("Done")
leg.go_home()