import math

L1 = 2.5
L2 = 10.0
L3 = 12.0

def leg_ik(x, y, z):
    hip_deg = math.degrees(math.atan2(y, x))

    reach = math.sqrt(x*x + y*y) - L1
    D = math.sqrt(reach*reach + z*z)

    cos_tib = (L2*L2 + L3*L3 - D*D) / (2 * L2 * L3)

    if abs(cos_tib) > 1.0:
        return None

    tibia_deg = math.degrees(math.acos(cos_tib))

    phi1 = math.atan2(-z, reach)

    cos_phi2 = (L2*L2 + D*D - L3*L3) / (2 * L2 * D)
    cos_phi2 = max(-1.0, min(1.0, cos_phi2))

    phi2 = math.acos(cos_phi2)

    femur_deg = math.degrees(phi1 - phi2)

    return hip_deg, femur_deg, tibia_deg


def ik_to_servo(hip_deg, femur_deg, tibia_deg):
    servo_hip = 90 + hip_deg
    servo_femur = 90 + femur_deg
    servo_tibia = 90 - tibia_deg

    servo_hip = max(0, min(180, servo_hip))
    servo_femur = max(0, min(180, servo_femur))
    servo_tibia = max(30, min(150, servo_tibia))

    return servo_hip, servo_femur, servo_tibia