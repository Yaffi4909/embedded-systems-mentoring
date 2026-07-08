import csv
import time
from ik_solver import leg_ik, ik_to_servo

positions = [
    ("P1_HOME", 12.5, 0, -8),
    ("P2_STAND", 12.5, 0, -10),
    ("P3_LIFT", 12.5, 0, -3),
    ("P4_EXTEND", 18.0, 0, -10),
    ("P5_RETRACT", 7.0, 0, -10),
    ("P6_FORWARD", 15.0, 0, -8),
    ("P7_BACK", 10.0, 0, -8),
    ("P8_UP", 12.5, 0, -5),
    ("P9_DOWN", 12.5, 0, -12),
    ("P10_SIDE", 12.5, 3, -8),
]

unreachable_positions = [
    ("BAD_1", 30, 0, -30),
    ("BAD_2", 50, 0, -5),
    ("BAD_3", 0, 0, -40),
    ("BAD_4", 25, 15, -20),
    ("BAD_5", -30, 0, -10),
]

with open("ik_test_results.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "name", "x", "y", "z",
        "hip_deg", "femur_deg", "tibia_deg",
        "servo_hip", "servo_femur", "servo_tibia",
        "status"
    ])

    for name, x, y, z in positions:
        angles = leg_ik(x, y, z)

        print("\n" + name)
        print("Target:", x, y, z)

        if angles is None:
            print("UNREACHABLE")
            writer.writerow([name, x, y, z, "", "", "", "", "", "", "UNREACHABLE"])
            continue

        hip, femur, tibia = angles
        servo_hip, servo_femur, servo_tibia = ik_to_servo(hip, femur, tibia)

        print("IK angles:", angles)
        print("Servo angles:", servo_hip, servo_femur, servo_tibia)

        writer.writerow([
            name, x, y, z,
            hip, femur, tibia,
            servo_hip, servo_femur, servo_tibia,
            "OK"
        ])

        time.sleep(1)
        
    print("\nTesting unreachable positions:")

    for name, x, y, z in unreachable_positions:
        angles = leg_ik(x, y, z)

        print("\n" + name)
        print("Target:", x, y, z)

        if angles is None:
            print("OK - unreachable detected")
        else:
            print("WARNING - should be unreachable but got:", angles)

print("\nDone. Results saved to ik_test_results.csv")