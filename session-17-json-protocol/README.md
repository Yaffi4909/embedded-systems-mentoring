# Session 17 – JSON Protocol + ESP32 Servo Control

## Overview

This project implements UART communication between a Raspberry Pi and an ESP32 using a JSON-based protocol.

The Raspberry Pi sends a `CMD` frame that contains 18 joint angles.
The ESP32 receives the frame, parses the JSON, updates the joint targets, and controls servos through a PCA9685 servo driver.

## System Flow

```text
Raspberry Pi
   ↓ UART JSON CMD
ESP32
   ↓ I2C
PCA9685
   ↓ PWM
Servos
```

## Hardware

* Raspberry Pi
* ESP32-C5
* PCA9685 16-channel PWM servo driver
* MG996R servo motor
* External 5V power supply for the servo
* Common GND between ESP32, PCA9685, and servo power supply

## Wiring

### UART

```text
RPi TX  -> ESP32 RX
RPi RX  -> ESP32 TX
GND     -> GND
```

ESP32 UART pins:

```cpp
#define RXD2 4
#define TXD2 5
```

### I2C – ESP32 to PCA9685

```text
ESP32 GPIO2 -> PCA9685 SDA
ESP32 GPIO3 -> PCA9685 SCL
ESP32 3V3   -> PCA9685 VCC
ESP32 GND   -> PCA9685 GND
```

### Servo Power

```text
External 5V -> PCA9685 V+
External GND -> PCA9685 GND
Servo signal -> PCA9685 channel 0
```

## Project Files

```text
session-17.ino        Main setup and loop
config.h              Pin definitions and servo constants
servo_driver.ino      PCA9685 setup and servo angle control
hex_leg.ino           HexLeg class
leg_controller.ino    Six-leg controller
uart_protocol.ino     UART JSON protocol parser
protocol.py           Raspberry Pi sender
```

## Protocol

Each UART message is a JSON frame ending with `\n`.

### CMD Frame

Sent from Raspberry Pi to ESP32:

```json
{
  "type": "CMD",
  "seq": 1,
  "ts": 123456,
  "data": {
    "joints": [90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90],
    "speed": 25
  }
}
```

### ACK Frame

Sent from ESP32 to Raspberry Pi after a valid command:

```json
{
  "type": "ACK",
  "seq": 1,
  "ts": 123500,
  "data": {
    "status": "OK",
    "applied": 18
  }
}
```

### ERR Frame

Sent from ESP32 when the received frame is invalid:

```json
{
  "type": "ERR",
  "seq": 0,
  "ts": 123500,
  "data": {
    "code": "BAD_JSON",
    "msg": "InvalidInput"
  }
}
```

## Servo Driver

The PCA9685 is initialized over I2C.

Servo angle is converted to PWM pulse width using the calibrated range:

```cpp
#define SERVO_MIN_US 550
#define SERVO_MAX_US 2670
#define SERVO_FREQ 50
```

The function:

```cpp
setServoAngle(channel, angle);
```

moves a servo on the selected PCA9685 channel.

## HexLeg Class

The `HexLeg` class represents one robot leg with three joints:

```text
hip
femur
tibia
```

Each leg stores three PCA9685 channels and supports:

```cpp
moveTo(hip, femur, tibia);
home();
stand();
setHip(angle);
setFemur(angle);
setTibia(angle);
moveByOffset(...);
printState(...);
```

## Six-Leg Controller

The project defines six legs:

```cpp
legs[0] -> channels 0, 1, 2
legs[1] -> channels 3, 4, 5
legs[2] -> channels 6, 7, 8
legs[3] -> channels 9, 10, 11
legs[4] -> channels 12, 13, 14
legs[5] -> channels 15, 16, 17
```

The function:

```cpp
updateLegs();
```

takes the 18 values from `g_joints[]` and sends them to the matching servo channels.

## ESP32 Program Flow

1. Start Serial and Serial2.
2. Initialize all joint angles to 90 degrees.
3. Initialize the PCA9685 servo driver.
4. Initialize all six legs.
5. Start the UART FreeRTOS task.
6. Wait for JSON commands from the Raspberry Pi.
7. Parse each CMD frame.
8. Validate that it contains exactly 18 joint angles.
9. Move the legs.
10. Send ACK back to the Raspberry Pi.

## Raspberry Pi Test

Run:

```bash
python protocol.py
```

Example output:

```text
Sending angle: 0
Sent CMD seq=1
Confirmed ACK seq=1

Sending angle: 10
Sent CMD seq=2
Confirmed ACK seq=2
```

## Expected ESP32 Serial Output

```text
I2C scanner started
Found I2C device at 0x40
Found I2C device at 0x70
Scan done
ESP32 UART JSON protocol ready
CMD received seq=1 speed=25
update legs
send ACK : seq = 1
```

## Current Status

* UART communication works.
* JSON CMD frames are received by the ESP32.
* ACK and ERR replies work.
* PCA9685 is detected over I2C.
* Servo angle commands are sent through the PCA9685.
* Code structure is split into multiple Arduino tabs for better organization.

