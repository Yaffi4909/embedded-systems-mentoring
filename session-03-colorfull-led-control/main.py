from machine import Pin, PWM, ADC
from time import sleep

print("It is colorfull!!!")

red_led = PWM(Pin(15), freq = 100)
green_led = PWM(Pin(7), freq = 100)
blue_led = PWM(Pin(6), freq = 100)

pot_red = ADC(Pin(1))
pot_green = ADC(Pin(2))
pot_blue = ADC(Pin(10))

pot_blue.atten(ADC.ATTN_11DB)   
pot_green.atten(ADC.ATTN_11DB)   
pot_red.atten(ADC.ATTN_11DB)  

servo = PWM(Pin(14), freq = 50)

while True:
    value_0 = pot_blue.read()
    duty = int(value_0 * 1023 / 4095)
    blue_led.duty(duty)

    value_1 = pot_green.read()
    duty = int(value_1 * 1023 / 4095)
    green_led.duty(duty)

    value_2 = pot_red.read()
    duty = int(value_2 * 1023 / 4095)
    red_led.duty(duty)

    total = value_0 + value_1 + value_2
    servo_duty = total * 180 /(4095 *3)
    servo_duty = int(26 + servo_duty * (128 - 26) / 180)

    servo.duty(servo_duty)
