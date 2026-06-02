

from machine import Pin
from neopixel import NeoPixel
import uasyncio as asyncio


NUM_LEDS_ON_CAR_TRAFIC = 3
# NUM_LEDS_ON_PEPOLE_TRAFIC = 2
PIN_UP = 13
PIN_DOWN = 14
PIN_LEFT = 27
PIN_RIGHT = 15

print("Hello, Traffic Light System!")


strip_0 = NeoPixel(Pin(PIN_LEFT), NUM_LEDS_ON_CAR_TRAFIC) #LEFT
strip_1 = NeoPixel(Pin(PIN_RIGHT), NUM_LEDS_ON_CAR_TRAFIC) #RIGHT
strip_2 = NeoPixel(Pin(PIN_DOWN), NUM_LEDS_ON_CAR_TRAFIC) #DOWN
strip_3 = NeoPixel(Pin(PIN_UP), NUM_LEDS_ON_CAR_TRAFIC) #UP

strips = [strip_0, strip_1, strip_2, strip_3]


def changeColor(color, traffic):
    if color == 1:
        strips[traffic][1] = (255, 120, 0)
    else :
        strips[traffic][0] = (255, 0, 0) if color == 0 else (0, 0, 0)
        strips[traffic][1] = (0, 0, 0)
        strips[traffic][2] = (0, 255, 0) if color == 2 else (0, 0, 0)
    strips[traffic].write()


async def traffic_task():
    ud = 0
    rl = 2
    while True:

        for i in range(4):
            changeColor(1, i)
      
        await asyncio.sleep(2)

        changeColor(rl, 0)
        changeColor(rl, 1)
        changeColor(ud, 2)
        changeColor(ud, 3)
        await asyncio.sleep(5)

        ud, rl = rl, ud
 

async def main():
    asyncio.create_task(traffic_task())

    while True:
        await asyncio.sleep(1)


asyncio.run(main())









