# Session 17 – Task 1: JSON Protocol Round-Trip

## Goal

The goal of this task is to implement a basic JSON-based UART protocol between a Raspberry Pi and an ESP32.

The Raspberry Pi sends a `CMD` frame to the ESP32, and the ESP32 replies with an `ACK` frame if the command was received and parsed successfully.

## Hardware

* Raspberry Pi
* ESP32
* UART connection between Raspberry Pi and ESP32

## UART Configuration

Both sides use the same UART settings:

```text
Baud rate: 115200
Line ending: \n
```

## Files

```text
protocol.py
uart_protocol.ino
```

## Protocol Flow

1. Raspberry Pi builds a JSON `CMD` frame.
2. Raspberry Pi sends the frame over UART.
3. ESP32 receives the frame.
4. ESP32 parses the JSON.
5. ESP32 checks that the frame contains exactly 18 joint values.
6. ESP32 sends an `ACK` frame back to the Raspberry Pi.
7. Raspberry Pi verifies that the `ACK` sequence number matches the sent command.

## CMD Frame

Example frame sent from Raspberry Pi to ESP32:

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

## ACK Frame

Example reply sent from ESP32 to Raspberry Pi:

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

## How to Run on Raspberry Pi

Activate the virtual environment:

```bash
source ~/session-14/venv/bin/activate
```

Run the Python file:

```bash
python protocol.py
```

## Expected Output on Raspberry Pi

```text
Sent CMD seq=1, attempt=1
Confirmed ACK seq=1
```

## Expected Output on ESP32 Serial Monitor

```text
CMD received seq=1 speed=25
joint[0] = 90.00
joint[1] = 90.00
...
joint[17] = 90.00
```

## Result

The round-trip communication works successfully:

```text
Raspberry Pi -> CMD -> ESP32
Raspberry Pi <- ACK <- ESP32
```

This confirms that the Raspberry Pi and ESP32 can communicate using a structured JSON protocol over UART.
