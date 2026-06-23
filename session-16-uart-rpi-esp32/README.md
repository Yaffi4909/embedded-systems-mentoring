# Session 16 – UART Communication Between Raspberry Pi and ESP32

## Overview

This session demonstrates UART serial communication between a Raspberry Pi and an ESP32.

The Raspberry Pi acts as the main controller and sends commands over UART.
The ESP32 receives the messages, processes them, and sends responses back to the Raspberry Pi.

The project includes two parts:

1. Basic UART echo communication
2. Sending 18 joint angle values from the Raspberry Pi to the ESP32

---

## Hardware Used

* Raspberry Pi 3B+
* ESP32-C5
* Jumper wires
* USB cable for ESP32 upload/debug
* UART connection between Raspberry Pi and ESP32

---

## UART Configuration

| Setting            | Value        |
| ------------------ | ------------ |
| Baud rate          | 115200       |
| Raspberry Pi port  | `/dev/ttyS0` |
| ESP32 UART         | `Serial2`    |
| Communication type | UART serial  |
| Logic level        | 3.3V         |

---

## Wiring

| Raspberry Pi | ESP32    |
| ------------ | -------- |
| GPIO14 TX    | ESP32 RX |
| GPIO15 RX    | ESP32 TX |
| GND          | GND      |

In my ESP32 code, the UART pins are defined as:

```cpp
#define RXD2 4
#define TXD2 5
```

So the wiring used in this project is:

```text
Raspberry Pi TX  -> ESP32 GPIO4 RX
Raspberry Pi RX  -> ESP32 GPIO5 TX
Raspberry Pi GND -> ESP32 GND
```

---

## Folder Structure

```text
session-16-uart-rpi-esp32/
│
├── rpi/
│   ├── rpi_uart.py
│   └── rpi_uart_angles_sender.py
│
├── esp32/
│   ├── esp32_uart_receiver/
│   │   └── esp32_uart_receiver.ino
│   │
│   └── esp32_uart_angles_receiver/
│       └── esp32_uart_angles_receiver.ino
│
├── screenshot_rpi_uart.png
└── screenshot_uart_engle.png
```

---

## Part 1 – Basic UART Echo Test

### Raspberry Pi

File:

```text
rpi/rpi_uart.py
```

This Python script opens `/dev/ttyS0` and sends a text message to the ESP32:

```text
HELLO TO ESP32 FROM RPI
```

It also reads and prints responses from the ESP32.

### ESP32

File:

```text
esp32/esp32_uart_receiver/esp32_uart_receiver.ino
```

This program uses two FreeRTOS tasks:

* `rx_task` – receives messages from the Raspberry Pi
* `tx_task` – sends a message back to the Raspberry Pi every second

The ESP32 sends back:

```text
hello to RPI from ESP32!!!
```

### Expected Output

On the Raspberry Pi:

```text
Sent
RX: hello to RPI from ESP32!!!
```

On the ESP32 Serial Monitor:

```text
RX from RPi: HELLO TO ESP32 FROM RPI
Send to RPi: hello to RPI from ESP32!!!
```

---

## Part 2 – Sending 18 Joint Angles

### Raspberry Pi

File:

```text
rpi/rpi_uart_angles_sender.py
```

This script sends 18 comma-separated joint angle values:

```text
90,60,120,90,60,120,90,60,120,90,60,120,90,60,120,90,60,120
```

### ESP32

File:

```text
esp32/esp32_uart_angles_receiver/esp32_uart_angles_receiver.ino
```

The ESP32 receives the string, splits it by commas, and stores the values in:

```cpp
float joint_angles[18];
```

If exactly 18 values are received, the ESP32 prints the table and sends back:

```text
{received:18,status:OK}
```

If the message format is not valid, it sends:

```text
{status:ERROR,reason:bad_format}
```

---

## How to Run

### On Raspberry Pi

Activate the virtual environment:

```bash
cd ~/session-16
```

Run the basic UART test:

```bash
python rpi_uart.py
```

Run the angles sender:

```bash
python rpi_uart_angles_sender.py
```

---

### On ESP32

Open Arduino IDE.

Upload one of the following sketches:

```text
esp32_uart_receiver.ino
```

or:

```text
esp32_uart_angles_receiver.ino
```

Open Serial Monitor at:

```text
115200 baud
```

---

## Results

The UART bridge was tested successfully.

The Raspberry Pi was able to send messages to the ESP32, and the ESP32 was able to send responses back to the Raspberry Pi.

The second test also sends 18 joint angles from the Raspberry Pi to the ESP32 and receives a confirmation message.

---

## What I Learned

* How UART communication works
* How to connect TX, RX, and GND correctly
* How to use `/dev/ttyS0` on Raspberry Pi
* How to use `pyserial` in Python
* How to use `Serial2` on ESP32
* How to create FreeRTOS tasks on ESP32
* How to send structured data between two boards
* How to parse comma-separated values on ESP32

---

## Status

Completed successfully.
