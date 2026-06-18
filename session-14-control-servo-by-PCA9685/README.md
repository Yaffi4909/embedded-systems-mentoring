# Session 14 — Servo Control with PCA9685 and Raspberry Pi

## Overview

This session demonstrates how to control a servo motor from a Raspberry Pi using the PCA9685 16-channel PWM driver over I2C.

The PCA9685 allows the Raspberry Pi to control servo motors using only two I2C lines:

* SDA
* SCL

In this session, the servo is connected to channel 0 of the PCA9685 driver.

## Hardware Used

* Raspberry Pi 3B+
* PCA9685 servo driver
* Servo motor
* Jumper wires
* 5V power connection for the servo driver

## Wiring

### Raspberry Pi to PCA9685

| PCA9685 Pin | Raspberry Pi Pin |
| ----------- | ---------------- |
| VCC / 3V3   | 3.3V             |
| GND         | GND              |
| SDA / P20   | GPIO2 / SDA      |
| SCL / P19   | GPIO3 / SCL      |
| VIN         | 5V               |
| VIN GND     | GND              |

### Servo Connection

The servo is connected to channel 0 on the PCA9685.

| Servo Wire      | PCA9685 Channel 0 |
| --------------- | ----------------- |
| Brown / Black   | GND               |
| Red             | V+                |
| Orange / Yellow | Signal            |

## I2C Check

Before running the Python code, verify that the Raspberry Pi detects the PCA9685:

```bash
sudo i2cdetect -y 1
```

Expected result:

```text
40
```

If address `0x40` appears, the PCA9685 is detected correctly.

## Python Environment Setup

Create and activate a virtual environment:

```bash
cd ~/embedded-systems-mentoring/session-14-control-servo-by-PCA9685
python3 -m venv venv
source venv/bin/activate
```

Install the required libraries:

```bash
pip install adafruit-circuitpython-pca9685
pip install adafruit-circuitpython-servokit
```

## Files in This Session

```text
session-14-control-servo-by-PCA9685/
├── task_1_servo_test.py
└── task_2_angle_input.py
```

## Task 1 — Servo Test

File:

```text
task_1_servo_test.py
```

This script tests basic servo movement on channel 0.

The servo moves between:

* 0 degrees
* 90 degrees
* 180 degrees

Run:

```bash
python3 task_1_servo_test.py
```

If permission or I2C access causes an error, run:

```bash
sudo ./venv/bin/python3 task_1_servo_test.py
```

## Task 2 — Angle Input from Terminal

File:

```text
task_2_angle_input.py
```

This script allows the user to type an angle in the terminal.

Valid input:

```text
0-180
```

The servo immediately moves to the selected angle.

The script also validates the input and prints an error message if:

* The input is not a number
* The angle is lower than 0
* The angle is higher than 180

Run:

```bash
python3 task_2_angle_input.py
```

To exit the program, type:

```text
q
```

## Servo Calibration

The servo pulse width range was set to:

```python
set_pulse_width_range(550, 2670)
```

This means:

* 550 microseconds = 0 degrees
* 2670 microseconds = 180 degrees

This range can be adjusted if the servo does not reach the expected movement range.

## Troubleshooting

### PCA9685 is not detected

Run:

```bash
sudo i2cdetect -y 1
```

If `40` does not appear:

* Check SDA and SCL wiring
* Check GND connection
* Check 3.3V logic power
* Check VIN power connection

### Error: No I2C device at address 0x40

This means the Raspberry Pi cannot communicate with the PCA9685.

Check:

* PCA9685 power
* Common GND
* I2C wiring
* I2C enabled in Raspberry Pi settings

### Servo does not move

Check:

* Servo is connected to channel 0
* VIN is connected to 5V
* GND is connected
* Servo wires are in the correct order
* PCA9685 is detected by `i2cdetect`

## What I Learned

* How to enable and test I2C on Raspberry Pi
* How to detect a PCA9685 driver using `i2cdetect`
* How to control a servo motor with Python
* How to use the Adafruit ServoKit library
* How to move a servo to a specific angle
* Why servo power must be handled separately from the signal lines

## Commands Summary

```bash
sudo i2cdetect -y 1
```

```bash
source venv/bin/activate
```

```bash
python3 task_1_servo_test.py
```

```bash
python3 task_2_angle_input.py
```

## Status

* PCA9685 detected on I2C address `0x40`
* Servo connected to channel 0
* Basic servo angle control implemented
