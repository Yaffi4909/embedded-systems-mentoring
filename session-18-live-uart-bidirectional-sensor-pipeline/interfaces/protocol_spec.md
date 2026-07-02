# UART Protocol Specification

## Overview

The ESP32 and Raspberry Pi communicate over UART using JSON frames.

Each frame is a single JSON object terminated by a newline (`\n`).

---

# STATUS Frame

The ESP32 periodically sends a STATUS frame to the Raspberry Pi every 2 seconds.

Example:

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

## Fields

| Field | Type | Description |
|-------|------|-------------|
| type | string | Frame type. Always `"STATUS"` |
| seq | integer | Sequential frame number |
| ts | integer | Timestamp in milliseconds |
| data | object | Sensor data |

---

## Sensor Data

| Field | Type | Units | Description |
|-------|------|-------|-------------|
| temp | float | °C | Temperature |
| hum | float | % | Relative humidity |
| dist | float | cm | Distance measured by the HC-SR04 sensor |
| bat | integer | % | Battery level (placeholder value) |

---

## Transmission Rate

The ESP32 sends one STATUS frame every 2 seconds.

---

## Example Flow

```
ESP32 ---------------------> Raspberry Pi
          STATUS
```

The Raspberry Pi should:

1. Read the UART line.
2. Parse the JSON.
3. Verify that `type == "STATUS"`.
4. Store the latest sensor values.
5. Update the live dashboard.