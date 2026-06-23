import serial
import time

ser = serial.Serial(
    port='/dev/ttyS0',
    baudrate=115200,
    timeout=1
)

print("UART open")

while True:
    ser.write(b"HELLO TO ESP32 FROM RPI\n")
    print("Sent")

    if ser.in_waiting:
        print("RX:", ser.readline().decode().strip())

    time.sleep(1)
