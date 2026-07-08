from ik_solver import leg_ik, ik_to_servo

positions = [
    ("HOME",    12.5, 0, -8),
    ("STAND",   12.5, 0, -10),
    ("LIFT",    12.5, 0, -3),
    ("EXTEND",  18.0, 0, -10),
    ("RETRACT", 7.0, 0, -10),
]

for name, x, y, z in positions:
    angles = leg_ik(x, y, z)

    print("\n" + name)
    print("Target:", x, y, z)

    if angles is None:
        print("UNREACHABLE")
        continue

    print("IK angles:", angles)

    servo_angles = ik_to_servo(*angles)
    print("Servo angles:", servo_angles)