from adafruit_servokit import ServoKit
import time

kit = ServoKit(channels=16)
servo = kit.servo[0]

servo.set_pulse_width_range(550, 2670)

print("Servo angle control - channel 0")
print("Type angle 0-180, or q to quit")

while True:
    user_input = input("Enter angle: ")

    if user_input.lower() == "q":
        print("Exit")
        break

    try:
        angle = int(user_input)
    except ValueError:
        print("Error: enter a number")
        continue

    if angle < 0 or angle > 180:
        print("Error: angle must be 0-180")
        continue

    print(f"Moving servo to {angle} degrees")
    servo.angle = angle
    time.sleep(0.5)
