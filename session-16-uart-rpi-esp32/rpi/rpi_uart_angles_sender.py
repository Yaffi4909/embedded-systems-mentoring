import serial
import time

ser = serial.Serial(
    port="/dev/ttyS0",
    baudrate=115200,
    timeout=1
)

angles = "90,60,120,90,60,120,90,60,120,90,60,120,90,60,120,90,60,120\n"

while True:
    ser.write(angles.encode())
    print("Sent:", angles.strip())

    if ser.in_waiting:
        reply = ser.readline().decode().strip()
        print("RX:", reply)

    time.sleep(1)
