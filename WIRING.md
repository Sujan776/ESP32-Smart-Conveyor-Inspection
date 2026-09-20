# Wiring — Smart Conveyor Inspection & Automatic Reject System

| Component | Connection |
|---|---|
| IR sensor + | 3V3 |
| IR sensor D | GPIO 26 |
| IR sensor - | GND |
| HC-SR04 VCC | 5V |
| HC-SR04 TRIG | GPIO 5 |
| HC-SR04 ECHO | GPIO 19 |
| HC-SR04 GND | GND |
| Product potentiometer VCC | 3V3 |
| Product potentiometer SIG | GPIO 34 |
| Product potentiometer GND | GND |
| OLED VCC | 3V3 |
| OLED GND | GND |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Green LED | GPIO 25 through 220 ohm resistor |
| Yellow LED | GPIO 33 through 220 ohm resistor |
| Red LED | GPIO 32 through 220 ohm resistor |
| Buzzer + | GPIO 23 |
| Buzzer - | GND |
| Servo signal | GPIO 18 |
| Servo V+ | 5V |
| Servo GND | GND |
| Pushbutton | GPIO 27 to GND, INPUT_PULLUP |

IR logic used by the firmware: LOW = object detected, HIGH = no object.
