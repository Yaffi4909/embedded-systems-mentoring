# Smart Weather Station

ROBOGREENO — Mini Project
Month 1 Capstone Project

## Project Overview

This project is a Smart Weather Station based on ESP32.

The system reads temperature, humidity, distance, presence, and light level.
The data is sent to Arduino Cloud Dashboard and is also advertised using BLE.

The project uses FreeRTOS tasks in order to run several actions at the same time.

## Project Features

* Read temperature and humidity from DHT22
* Read distance using HC-SR04
* Detect presence when distance is less than 50 cm
* Read ambient light using LDR
* Send live data to Arduino Cloud Dashboard
* Advertise sensor data using BLE
* Send HTTP POST updates using WiFi
* Print a weather report to Serial Monitor
* Use FreeRTOS multitasking

## Hardware Components

| Component    | Quantity | Purpose                         |
| ------------ | -------- | ------------------------------- |
| ESP32 DevKit | 1        | Main MCU                        |
| DHT22 Sensor | 1        | Temperature and humidity        |
| HC-SR04      | 1        | Distance and presence detection |
| LDR          | 1        | Ambient light level             |
| Resistors    | 4        | Voltage divider and LDR divider |
| Blue LED     | 1        | Presence indicator              |
| Yellow LED   | 1        | Cloud sync indicator            |

## Wiring

### DHT22

| DHT22 | ESP32                    |
| ----- | ------------------------ |
| VCC   | 3.3V                     |
| GND   | GND                      |
| DATA  | GPIO defined in the code |

Note:
The DHT22 code is implemented, but the physical DHT22 sensor was not connected during real hardware testing.

### HC-SR04

| HC-SR04 | ESP32                                            |
| ------- | ------------------------------------------------ |
| VCC     | 5V                                               |
| GND     | GND                                              |
| TRIG    | GPIO defined in the code                         |
| ECHO    | GPIO defined in the code through voltage divider |

Important:
The HC-SR04 ECHO pin outputs 5V.
ESP32 GPIO input supports 3.3V.
Therefore, the ECHO pin must be connected through a voltage divider.

### LDR

The LDR is connected as a voltage divider:

```text
3.3V → LDR → ADC pin → 10k resistor → GND
```

The analog value is mapped to 0–100%.

## Software Architecture

The project uses FreeRTOS tasks.

| Task Name    | Responsibility                         |
| ------------ | -------------------------------------- |
| sensorTask   | Reads DHT22, HC-SR04, and LDR          |
| cloudTask    | Updates Arduino Cloud variables        |
| bleTask      | Advertises sensor data using BLE       |
| presenceTask | Detects presence and controls blue LED |
| displayTask  | Prints sensor report to Serial Monitor |
| wifiTask     | Sends HTTP POST data                   |

## Arduino Cloud Variables

| Variable Name  | Type  | Purpose                |
| -------------- | ----- | ---------------------- |
| temperature    | float | Temperature value      |
| humidity       | float | Humidity value         |
| distance       | float | Distance in cm         |
| lightLevel     | int   | Light level in percent |
| personDetected | bool  | Presence status        |

## Presence Detection

Presence is detected when:

```text
distance > 0 && distance < 50 cm
```

To avoid false detection, the system requires 3 consecutive readings before setting presence to true.

When presence is detected:

* Blue LED turns ON
* `personDetected` becomes true
* The value is updated in Arduino Cloud

## BLE Advertising

The ESP32 advertises the sensor data using BLE.

The BLE data includes:

* Distance
* Temperature
* Humidity
* Light level

Example format:

```json
{
  "distance": 35.2,
  "temperature": 24.5,
  "humidity": 52.0,
  "light": 80
}
```

## HTTP POST

The project sends sensor data to a logging server using HTTP POST.

The POST data includes:

* Temperature
* Humidity
* Distance
* Light level
* Presence status

## Required Repository Structure

The files uploaded to GitHub should be organized according to the project instructions:

```text
robogreeno-emb/
└── mini-project/
    ├── starter_code/
    │   └── starter_code.ino
    │
    ├── final_code/
    │   ├── final_code.ino
    │   └── thingProperties.h
    │
    ├── wokwi/
    │   ├── diagram.json
    │   └── libraries.txt
    │
    ├── README.md
    └── dashboard_screenshot.png
```

## Files Explanation

| File / Folder                 | Description                             |
| ----------------------------- | --------------------------------------- |
| starter_code/starter_code.ino | Initial starter version of the project  |
| final_code/final_code.ino     | Final Arduino code                      |
| final_code/thingProperties.h  | Arduino Cloud generated properties file |
| wokwi/diagram.json            | Wokwi circuit diagram                   |
| wokwi/libraries.txt           | Required Wokwi libraries                |
| README.md                     | Project documentation                   |
| dashboard_screenshot.png      | Screenshot of Arduino Cloud Dashboard   |

## Secret Files

The file `arduino_secrets.h` contains private WiFi and Arduino Cloud credentials.

This file should NOT be uploaded to GitHub.

Add it to `.gitignore`:

```gitignore
arduino_secrets.h
```

Example:

```cpp
#define SECRET_SSID "your_wifi_name"
#define SECRET_OPTIONAL_PASS "your_wifi_password"
#define SECRET_DEVICE_KEY "your_device_key"
```

## How to Run

1. Open Arduino IDE.
2. Select the correct ESP32 board.
3. Open `final_code/final_code.ino`.
4. Make sure `thingProperties.h` is in the same folder.
5. Add `arduino_secrets.h` locally with WiFi and Arduino Cloud credentials.
6. Install the required libraries.
7. Upload the code to the ESP32.
8. Open Serial Monitor.
9. Open Arduino Cloud Dashboard.
10. Check that the values update live.

## Required Libraries

* ArduinoIoTCloud
* Arduino_ConnectionHandler
* DHT sensor library
* BLE library
* HTTPClient

## Testing

Tested:

* LDR reading
* HC-SR04 distance reading
* Presence detection
* Blue LED indicator
* Arduino Cloud variables
* BLE advertising
* HTTP POST structure
* Serial Monitor output

Not physically tested:

* DHT22 sensor, because the real sensor was not available.

The DHT22 code is included and ready for use when the sensor is connected.

## Submission Checklist

| Requirement                                           | Status |
| ----------------------------------------------------- | ------ |
| All 3 sensors implemented                             | Done   |
| FreeRTOS tasks implemented                            | Done   |
| Arduino Cloud dashboard with 5 variables              | Done   |
| Presence detection implemented                        | Done   |
| BLE advertising implemented                           | Done   |
| GitHub repository structure according to instructions | Done   |
| README file included                                  | Done   |
| Dashboard screenshot included                         | To add |

## Author

Student: Yaffi Yud
Project: Smart Weather Station
Course: ROBOGREENO Mini Project
Mentor: Dosithee Miet
