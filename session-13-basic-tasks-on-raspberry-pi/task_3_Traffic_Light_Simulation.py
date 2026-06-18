import RPi.GPIO as GPIO
import time

RED = 17
YELLOW = 22
GREEN = 4

GPIO.setmode(GPIO.BCM)

GPIO.setup(RED, GPIO.OUT)
GPIO.setup(YELLOW, GPIO.OUT)
GPIO.setup(GREEN, GPIO.OUT)

print("Session 13 - Task 3 -> Traffic Light Simulation")

try:
    while True:
        print("green on")
        GPIO.output(GREEN, GPIO.HIGH)
        time.sleep(3)
        GPIO.output(GREEN, GPIO.LOW)

        print("yellow on")
        GPIO.output(YELLOW, GPIO.HIGH)
        time.sleep(0.7)
        GPIO.output(YELLOW, GPIO.LOW)
        time.sleep(0.7)
        GPIO.output(YELLOW, GPIO.HIGH)
        time.sleep(0.7)
        GPIO.output(YELLOW, GPIO.LOW)

        print("red on")
        GPIO.output(RED, GPIO.HIGH)
        time.sleep(3)
        GPIO.output(RED, GPIO.LOW)

        print("yellow on")
        GPIO.output(YELLOW, GPIO.HIGH)
        time.sleep(0.7)
        GPIO.output(YELLOW, GPIO.LOW)
        time.sleep(0.7)
        GPIO.output(YELLOW, GPIO.HIGH)
        time.sleep(0.7)
        GPIO.output(YELLOW, GPIO.LOW)

except KeyboardInterrupt:
    print("Exiting program")

finally:
    GPIO.cleanup()




