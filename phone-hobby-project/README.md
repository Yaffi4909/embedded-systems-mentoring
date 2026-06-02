# Environmental Monitoring System (DHT22, I2C LCD & RGB LED)

A real-time environmental monitoring system written in **C++ (Arduino framework)** and simulated on the **Wokwi** platform. This project reads temperature and humidity levels, displays them on an LCD screen, and uses an RGB LED to provide instant visual feedback based on weather thresholds.

## Overview
This project demonstrates how to interface digital sensors with an LCD display and multi-color LED indicators using C++. It is designed to act as a smart thermostat or climate monitor indicator.

### Features
* **Real-time Climate Sensing:** Continuous sampling of temperature and humidity via the DHT22 sensor.
* **I2C LCD Display:** Clear text output showing current temperature ($^\circ\text{C}$) and humidity ($\%$) levels.
* **Visual Alert System:** The RGB LED changes colors dynamically to represent different climate zones (e.g., Cold, Normal, Hot).

---

## System Architecture & Pin Configuration

To replicate or review the hardware setup in the simulation, the components are mapped as follows:

| Component | Component Pin | Microcontroller GPIO | Description |
| :--- | :--- | :--- | :--- |
| **DHT22** | SDA / Data | GPIO X | Sensor data line |
| **I2C LCD 16x2** | SCL | GPIO X | I2C Clock Line |
| | SDA | GPIO X | I2C Data Line |
| **RGB LED** | Red Anode/Cathode | GPIO X | Controls Red channel |
| | Green Anode/Cathode| GPIO X | Controls Green channel |
| | Blue Anode/Cathode | GPIO X | Controls Blue channel |

*(Note: Please verify the exact GPIO numbers in your main source code and update the 'X' values in the table above accordingly).*

---

## How to Run the Project

### Option 1: Live Simulation
You can view, edit, and run the project directly inside your browser:
* [Live Wokwi Simulation](https://wokwi.com/projects/463973597513690113)

### Option 2: Local Setup / Custom Simulation
1. Copy the C++ source code from your main sketch file.
2. Open a new Arduino/C++ project in [Wokwi](https://wokwi.com/).
3. Ensure you add the required libraries (`DHT sensor library` and `LiquidCrystal I2C`) in the Library Manager tab.
4. Click the **Start Simulation** button.
5. Click on the **DHT22 sensor** during the simulation to manually slide and adjust temperature/humidity values and watch the LCD and RGB LED react instantly.

---

## Technologies & Components Used
* **Language:** C++ (Arduino Framework)
* **Hardware Elements:**
  * DHT22 Temperature & Humidity Sensor
  * I2C LCD Display (16x2)
  * RGB LED & Current-Limiting Resistors
* **Simulation Environment:** Wokwi
