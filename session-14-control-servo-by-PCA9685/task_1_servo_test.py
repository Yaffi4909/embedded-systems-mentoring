from adafruit_servokit import ServoKit
import time

kit = ServoKit(channels=16)

kit.servo[0].set_pulse_width_range(550, 2670)

print("0 degrees")
kit.servo[0].angle = 0
time.sleep(2)

print("90 degrees")
kit.servo[0].angle = 90
time.sleep(2)

print("180 degrees")
kit.servo[0].angle = 180
time.sleep(2)

print("Done")
