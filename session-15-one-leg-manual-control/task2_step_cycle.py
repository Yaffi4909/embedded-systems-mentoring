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
    HexLeg.STAND,
    HexLeg.LIFT,
    HexLeg.EXTEND,
    HexLeg.STAND,
    HexLeg.RETRACT,
    HexLeg.STAND,
]

NUM_CYCLES = 10

print("Homework Task 2")
print(f"Running {NUM_CYCLES} step cycles")

start_time = time.perf_counter()

try:
    for cycle in range(NUM_CYCLES):

        print(f"Cycle {cycle + 1}")

        for position in STEP_CYCLE:
            leg0.smooth_to(position, steps=25, delay=0.012)

        time.sleep(0.3)

finally:
    leg0.move_to(HexLeg.HOME)

end_time = time.perf_counter()

total_time = end_time - start_time
avg_cycle_time = total_time / NUM_CYCLES
cycles_per_minute = 60 / avg_cycle_time

print()
print(f"Total time: {total_time:.2f} sec")
print(f"Average cycle: {avg_cycle_time:.2f} sec")
print(f"Cycles per minute: {cycles_per_minute:.2f}")
