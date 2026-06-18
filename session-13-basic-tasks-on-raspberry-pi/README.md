# Session 13 – Raspberry Pi GPIO Basics

## Overview

This session introduces Raspberry Pi GPIO programming using Python.

The exercises cover:

* GPIO output control
* LED blinking
* Button input handling
* PWM (Pulse Width Modulation)
* Traffic light simulation
* Raspberry Pi hardware setup

---

## Tasks

### Task 1 – LED Blink

A simple Python application that blinks an LED connected to GPIO17.

Concepts learned:

* GPIO setup
* Output pins
* Infinite loops
* Timing with `time.sleep()`
* Proper GPIO cleanup

File:

```text
task_1_led_blink.py
```

---

### Task 2 – LEDs and Buttons

Control LEDs using physical push buttons.

Features:

* Two LEDs
* Two buttons
* Button press detection
* LED state control
* PWM brightness adjustment

File:

```text
task_2_leds_and_buttons.py
```

---

### Task 3 – Traffic Light Simulation

A Raspberry Pi implementation of a traffic light controller.

Sequence:

```text
Green  -> 3 seconds
Yellow -> 1 second
Red    -> 3 seconds
Repeat
```

Concepts learned:

* Multiple GPIO outputs
* State transitions
* Timing control
* Real hardware interaction

File:

```text
task_3_Traffic_Light_Simulation.py
```

---

## Hardware Used

* Raspberry Pi 3B+
* Breadboard
* LEDs
* Push buttons
* Resistors
* Jumper wires

---

## Software

* Raspberry Pi OS
* Python 3
* RPi.GPIO Library

Installation:

```bash
sudo apt install python3-rpi.gpio python3-gpiozero -y
```

---

## Running the Programs

Run a Python file:

```bash
python3 task_1_led_blink.py
```

Example:

```bash
python3 task_2_leds_and_buttons.py
```

```bash
python3 task_3_Traffic_Light_Simulation.py
```

Stop execution:

```text
Ctrl + C
```

---

## Learning Outcomes

By completing this session I learned:

* Raspberry Pi GPIO fundamentals
* Digital outputs
* Digital inputs
* Button handling
* PWM control
* Basic hardware debugging
* Python programming on Raspberry Pi
* Safe GPIO cleanup practices

---

## Repository

Part of the Embedded Systems Mentoring program.

Session 13 focuses on the transition from simulation environments to real Raspberry Pi hardware.
