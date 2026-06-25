# Session 15 – One Leg Manual Control

## Overview

In this session, a single hexapod robot leg was implemented using three servos connected to a PCA9685 PWM driver and controlled from a Raspberry Pi.

The leg consists of three joints:

* HIP – horizontal rotation
* FEMUR – leg lift and lowering
* TIBIA – leg extension and retraction

The goal of this session was to build a reusable software architecture for controlling a robot leg and to create smooth movements between predefined positions.

---

## Hardware

* Raspberry Pi 3B+
* PCA9685 16-Channel PWM Driver
* 3 Servo Motors
* External 5V Power Supply

### Servo Channel Assignment

| Joint | PCA9685 Channel |
| ----- | --------------- |
| HIP   | 0               |
| FEMUR | 1               |
| TIBIA | 2               |

---

## Software Structure

### leg.py

Contains the `HexLeg` class.

Features:

* Servo initialization
* Pulse width calibration
* Predefined leg positions
* Direct movement with `move_to()`
* Smooth interpolation with `smooth_to()`

### main.py

Demonstrates a complete step cycle using the predefined positions.

---

## Predefined Positions

| Position | Hip | Femur | Tibia |
| -------- | --- | ----- | ----- |
| HOME     | 90  | 90    | 90    |
| STAND    | 90  | 60    | 120   |
| LIFT     | 90  | 30    | 90    |
| EXTEND   | 135 | 60    | 120   |
| RETRACT  | 45  | 60    | 120   |

---

## Homework Task 1 – Custom Positions

Created and tested three additional leg positions:

### CROUCH

```python
(90, 120, 60)
```

### HIGH_LIFT

```python
(90, 20, 80)
```

### FORWARD_REACH

```python
(150, 60, 120)
```

Implemented in:

```text
task1_positions.py
```

---

## Homework Task 2 – Step Cycle Timing

Implemented a complete walking cycle repeated multiple times.

Measurements performed:

* Total execution time
* Average cycle time
* Cycles per minute

Implemented in:

```text
task2_step_cycle.py
```

---

## Homework Task 3 – Keyboard Control

Interactive control of the leg using keyboard commands.

### Commands

| Key | Action  |
| --- | ------- |
| H   | HOME    |
| W   | LIFT    |
| S   | STAND   |
| A   | RETRACT |
| D   | EXTEND  |
| X   | EXIT    |

Implemented in:

```text
task3_keyboard_control.py
```

---

## Skills Practiced

* Object-Oriented Programming in Python
* Servo motor control
* PCA9685 PWM driver usage
* Motion interpolation
* Robot leg kinematics basics
* Raspberry Pi hardware control
* Structured embedded software design

---

## Result

A fully controllable single hexapod leg capable of:

* Smooth movement
* Position-based control
* Keyboard interaction
* Reusable software architecture for future multi-leg robot development
