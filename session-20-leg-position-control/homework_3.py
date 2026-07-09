import time
import json
from adafruit_servokit import ServoKit
from leg_controller import LegController
from trajectory import STEP_CYCLE
from protocol import UARTProtocol

kit = ServoKit(channels=16)

prot = UARTProtocol(
    port="/dev/ttyS0",
    baudrate=115200,
    timeout=0.5
)

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

        ok = leg.move_foot_to(*pos, steps=25)

        if ok:
            joints_18 = [90] * 18
            joints_18[0] = kit.servo[0].angle
            joints_18[1] = kit.servo[1].angle
            joints_18[2] = kit.servo[2].angle

        ok_uart = prot.send_cmd_with_retry(joints_18, speed=25)

        if ok_uart:
            print("UART CMD confirmed")
        else:
            print("UART CMD failed")

        time.sleep(0.15)

print("Done")
leg.go_home()
prot.close()