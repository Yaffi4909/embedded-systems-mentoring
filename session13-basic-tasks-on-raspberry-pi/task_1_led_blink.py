import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)

print("task 1 in session 13 -> led blink -> is running")

try:
    while True:
        GPIO.output(17, GPIO.HIGH)
        time.sleep(0.5)

        GPIO.output(17, GPIO.LOW)
        time.sleep(0.5)

except KeyboardInterrupt:
    print("\nProgram stopped by user")

finally:
    GPIO.cleanup()
