# Session 10 – BLE Advertising with Sensor Data

## Overview

In this session I learned how to use Bluetooth Low Energy (BLE) on the ESP32 to broadcast sensor data wirelessly.

The project combines BLE Advertising with sensor readings and FreeRTOS tasks. The ESP32 advertises temperature, humidity, and distance values that can be viewed by nearby BLE devices.

---

## Objectives

* Understand BLE fundamentals
* Learn the difference between BLE and Classic Bluetooth
* Create BLE Services and Characteristics
* Broadcast sensor data using BLE Advertising
* Use FreeRTOS tasks to manage multiple operations simultaneously
* Display sensor data through BLE without wired communication

---

## Hardware

* ESP32
* DHT22 Temperature and Humidity Sensor
* HC-SR04 Ultrasonic Distance Sensor
* LED
* USB Cable

---

## Software

* Arduino IDE
* ESP32 BLE Library
* FreeRTOS
* Wokwi Simulator

---

## Features

### Sensor Task

Reads:

* Temperature
* Humidity
* Distance

Updates global variables periodically.

### BLE Task

Creates:

* BLE Service
* BLE Characteristics

Broadcasts sensor values through BLE.

### Display Task

Prints formatted sensor data to the Serial Monitor.

### LED Task

Blinks an LED to indicate the system is running.

---

## BLE Data Format

Example advertised data:

```text
T:25.1 H:60.2 D:30.5cm
```

Example JSON format:

```json
{
  "T":25.1,
  "H":60.2,
  "D":30.5
}
```

---

## FreeRTOS Tasks

| Task | Purpose | Priority |
|------|---------|----------|
| sensorTask | Read sensors | 1 |
| bleTask | Update BLE data | 1 |
| displayTask | Serial output | 2 |
| blinkTask | LED indication | 0 |

---

## BLE Configuration

### Service UUID

```cpp
4fafc201-1fb5-459e-8fcc-c5c9c331914b
```

### Characteristics

* Temperature Characteristic
* Humidity Characteristic
* Distance Characteristic

---

## What I Learned

* How BLE Advertising works
* Difference between GAP and GATT
* Creating BLE Services and Characteristics
* Updating BLE values dynamically
* Using notify() for BLE updates
* Managing multiple tasks with FreeRTOS
* Broadcasting sensor information wirelessly

---

## Repository Structure

```text
session-10-add-ble-to-sensor/
│
└── Untitled_jun07a/
   │
   ├── Untitled_jun07a.ino
   ├── arduino_secrets.h
   ├── README.md
   ├── thingProperties.h
   └── sketch.json
```

---

## Result

The ESP32 successfully broadcasts live sensor readings using BLE. Sensor data is updated continuously and can be viewed through BLE monitoring tools or simulation environments.

---

## Author

Session 10 – BLE Advertising with Sensor Data

Embedded Systems Bootcamp
