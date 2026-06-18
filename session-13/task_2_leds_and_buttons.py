#!/usr/bin/env python3

import RPi.GPIO as GPIO
import time

LED1 = 17
LED2 = 22

BUTTON1 = 27
BUTTON2 = 5

GPIO.setmode(GPIO.BCM)

GPIO.setup(LED1, GPIO.OUT)
GPIO.setup(LED2, GPIO.OUT)

GPIO.setup(BUTTON1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BUTTON2, GPIO.IN, pull_up_down=GPIO.PUD_UP)

pwm = GPIO.PWM(LED2, 1000)
pwm.start(0)

led1_state = False
brightness = 0

prev_button1 = GPIO.HIGH
prev_button2 = GPIO.HIGH

print("Session 13 - Task 2 : control leds by buttons")
print("Button1 -> Toggle LED1")
print("Button2 -> Increase LED2 brightness")

try:
    while True:
        button1 = GPIO.input(BUTTON1)
        button2 = GPIO.input(BUTTON2)

        if prev_button1 == GPIO.HIGH and button1 == GPIO.LOW:
            led1_state = not led1_state
            GPIO.output(LED1, led1_state)

            if led1_state:
                print("LED1 ON")
            else:
                print("LED1 OFF")

        if prev_button2 == GPIO.HIGH and button2 == GPIO.LOW:
            brightness += 10
    
            if brightness > 100:
                brightness = 0
    
            pwm.ChangeDutyCycle(brightness)
    
            print(f"LED2 Brightness = {brightness}%")
    
        prev_button1 = button1
        prev_button2 = button2

        time.sleep(0.05)

except KeyboardInterrupt:
    print("Stopped")

finally:
    pwm.stop()
    GPIO.cleanup()
