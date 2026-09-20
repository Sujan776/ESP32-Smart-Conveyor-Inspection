# ESP32 Smart Conveyor Inspection & Automatic Reject System

An ESP32-based industrial conveyor inspection prototype simulated using Wokwi and PlatformIO.

## Overview

The system simulates a conveyor inspection station. An IR sensor detects a product, a potentiometer represents a product quality parameter, and the ESP32 classifies the product as GOOD or DEFECTIVE. A servo acts as an automatic reject arm for defective products.

## Features

- IR-based product detection
- Simulated product quality parameter
- GOOD / DEFECTIVE classification
- Automatic servo-based rejection
- SSD1306 OLED monitoring
- Green / yellow / red status indicators
- Buzzer alert for defective products
- Total, good and rejected product counters
- Start/Stop pushbutton
- HC-SR04 range monitoring
- Wokwi + PlatformIO simulation

## Quality Logic

- Product value 30–70: GOOD
- Product value below 30 or above 70: DEFECTIVE
- GOOD: green LED, buzzer off, servo at 0 degrees
- DEFECTIVE: red LED, short buzzer alert, servo moves to 110 degrees and returns to 0 degrees

## Hardware / Wokwi Components

- ESP32 DevKit
- IR sensor
- HC-SR04 ultrasonic sensor
- Potentiometer
- SSD1306 OLED
- Servo motor
- Green LED
- Yellow LED
- Red LED
- 220 ohm resistors
- Buzzer
- Pushbutton

## GPIO Mapping

| Function | GPIO |
|---|---:|
| IR sensor | 26 |
| HC-SR04 TRIG | 5 |
| HC-SR04 ECHO | 19 |
| Product potentiometer | 34 |
| OLED SDA | 21 |
| OLED SCL | 22 |
| Green LED | 25 |
| Yellow LED | 33 |
| Red LED | 32 |
| Buzzer | 23 |
| Servo | 18 |
| Pushbutton | 27 |

## Project Structure

```text
ESP32-Smart-Conveyor-Inspection/
├── src/
│   └── main.cpp
├── diagram.json
├── wokwi.toml
├── platformio.ini
├── libraries.txt
├── WIRING.md
├── README.md
└── .gitignore
```

## Running in PlatformIO + Wokwi

1. Open this folder in VS Code.
2. Build with PlatformIO.
3. Ensure `diagram.json` and `wokwi.toml` are in the project root.
4. Start the Wokwi simulator.
5. Press the pushbutton to start the system.
6. Adjust the product potentiometer and trigger the IR sensor to test GOOD and REJECT conditions.

## Portfolio Note

This is a Wokwi-simulated industrial automation prototype. The servo represents a physical conveyor reject mechanism; the potentiometer represents a simulated product-quality measurement.

## Author

Sri Sujan VM
