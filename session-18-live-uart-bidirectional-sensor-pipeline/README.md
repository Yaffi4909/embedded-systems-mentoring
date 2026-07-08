# Session 18 – Live UART Bidirectional Sensor Pipeline

## Overview

This project implements a bidirectional UART communication pipeline between a Raspberry Pi and an ESP32.

The Raspberry Pi sends servo commands to the ESP32 using JSON `CMD` frames.
The ESP32 receives the commands, updates the servo targets, and sends back `ACK` or `ERR` frames.

At the same time, the ESP32 reads sensor data and sends live `STATUS` frames to the Raspberry Pi every 2 seconds.

The Raspberry Pi receives the live sensor data, displays it in a terminal dashboard, logs it to a CSV file, and can generate a graph using Python and matplotlib.

---

## System Flow

```text
Raspberry Pi  ---- CMD ---->  ESP32
Raspberry Pi  <--- ACK ----  ESP32
Raspberry Pi  <-- STATUS --  ESP32
```

---

## Hardware

* Raspberry Pi
* ESP32-C5
* PCA9685 servo driver
* MG996R servo motors
* DHT22 temperature and humidity sensor
* HC-SR04 distance sensor
* UART connection between Raspberry Pi and ESP32
* Common GND between all components

---

## Project Structure

```text
session-18-live-uart-bidirectional-sensor-pipeline/
├── ESP32/
│   ├── config.h
│   ├── session-18.ino
│   ├── uart_protocol.ino
│   ├── sensor.ino
│   ├── servo_driver.ino
│   ├── leg_controller.ino
│   └── hex_leg.ino
│
├── RPi/
│   ├── protocol.py
│   ├── rpi_full.py
│   ├── logger.py
│   └── plot_sensor_data.py
│
└── interfaces/
    └── protocol_spec.md
```

---

## UART Protocol

Each UART message is a JSON frame ending with a newline character.

### CMD Frame

Sent from the Raspberry Pi to the ESP32.

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

Sent from the ESP32 after a valid command.

```json
{
  "type": "ACK",
  "seq": 1,
  "ts": 123500,
  "data": {
    "status": "OK"
  }
}
```

### ERR Frame

Sent from the ESP32 when a frame is invalid.

```json
{
  "type": "ERR",
  "seq": 1,
  "ts": 123500,
  "data": {
    "code": "BAD_JSON",
    "msg": "Invalid input"
  }
}
```

### STATUS Frame

Sent from the ESP32 to the Raspberry Pi every 2 seconds.

```json
{
  "type": "STATUS",
  "seq": 12,
  "ts": 24502,
  "data": {
    "temp": 24.5,
    "hum": 62.1,
    "dist": 31.2,
    "bat": 85
  }
}
```

---

## ESP32 Side

The ESP32 runs several tasks:

### `sensorTask`

Reads sensor values from:

* DHT22
* HC-SR04

The values are stored in global variables.

### `statusTask`

Builds a `STATUS` JSON frame and sends it to the Raspberry Pi every 2 seconds.

### `uartTask`

Receives JSON frames from the Raspberry Pi.

It handles:

* `CMD`
* `ALERT`
* invalid frames

### `servoTask`

Reads the latest joint target values and updates the servos through the PCA9685.

---

## Raspberry Pi Side

### `protocol.py`

Handles UART communication.

Responsibilities:

* Send `CMD` frames
* Receive `ACK`, `ERR`, and `STATUS` frames
* Store the latest sensor values
* Run a background receiver thread

### `rpi_full.py`

Main program.

Responsibilities:

* Send servo commands to the ESP32
* Display live sensor data
* Check alert conditions
* Save sensor data to CSV

### `logger.py`

Saves received sensor data into:

```text
sensor_log.csv
```

CSV columns:

```text
seq, ts, temp, hum, dist, bat
```

### `plot_sensor_data.py`

Reads `sensor_log.csv` and creates a graph using matplotlib.

Output:

```text
sensor_plot.png
```

---

## Alert System

The Raspberry Pi checks the latest sensor values.

If:

```text
temp > 30
```

or:

```text
dist < 15
```

then the Raspberry Pi sends an `ALERT` frame to the ESP32.

The ESP32 receives the alert and blinks the alert LED 3 times.

---

## Running on Raspberry Pi

Activate the virtual environment:

```bash
source venv/bin/activate
```

Run the main program:

```bash
python rpi_full.py
```

After at least 1 minute of data collection, generate the graph:

```bash
python plot_sensor_data.py
```

---

## Expected Output

Example terminal dashboard:

```text
=== RoboGreeno Live Sensor Dashboard ===
seq :     12
temp:   24.5 C
hum :   62.1 %
dist:   31.2 cm
bat :     85 %
ts  :  24502 ms
```

---

## Homework Completed

* ESP32 reads live sensor data
* ESP32 sends `STATUS` frames every 2 seconds
* Raspberry Pi receives live sensor data
* Raspberry Pi displays a live dashboard
* Raspberry Pi sends servo `CMD` frames
* ESP32 returns `ACK`
* Alert system over UART was added
* Sensor data is logged to CSV
* Sensor data can be plotted using matplotlib
* Protocol documentation was added in `interfaces/protocol_spec.md`

---

## Commit Message

```text
Session 18 bidirectional UART live sensor pipeline
```
