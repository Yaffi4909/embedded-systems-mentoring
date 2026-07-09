import time
from adafruit_servokit import ServoKit

from leg_controller import LegController
from trajectory import waypoint_path


kit = ServoKit(channels=16)

leg = LegController(
    kit=kit,
    hip_ch=0,
    fem_ch=1,
    tib_ch=2
)


WAYPOINTS = [
    (10.0, 0, -10.0),
    (12.0, 0, -10.0),
    (14.0, 0, -10.0),
    (16.0, 0, -10.0),

    (16.0, 0, -8.0),
    (16.0, 0, -6.0),

    (14.0, 0, -6.0),
    (12.0, 0, -6.0),
    (10.0, 0, -6.0),

    (10.0, 0, -10.0)
]


print("Starting Homework 2 trajectory")

for i, point in enumerate(WAYPOINTS, start=1):
    print(f"Waypoint {i}: {point}")

waypoint_path(
    leg,
    WAYPOINTS,
    steps=20,
    delay=0.012
)

print("Trajectory completed")

leg.go_home()