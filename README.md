# XIAO ESP32C3_OLED_ThingSpeak
Arduino code for Seeed Studio XIAO ESP32-C3 with an OLED display. Fetches sensor data from ThingSpeak and displays it dynamically on the screen. Includes Wi-Fi connectivity and JSON data parsing.


# XIAO ESP32-C3 with OLED Display and ThingSpeak Integration

This repository contains Arduino code for interfacing a Seeed Studio XIAO ESP32-C3 microcontroller with an OLED display to fetch and display data from a ThingSpeak channel. The code dynamically retrieves sensor values, such as lux, temperature, humidity, air pressure, altitude, and noise level, from a ThingSpeak API and displays them on the OLED screen.

## Features

- **Wi-Fi Connectivity:** Connects to a Wi-Fi network for real-time data fetching.
- **ThingSpeak API Integration:** Fetches the latest sensor data from a specified ThingSpeak channel.
- **Dynamic Display:** Displays sensor readings with appropriate units and formats on the OLED screen.
- **Error Handling:** Displays an error message in case of data fetch or parsing failures.

## Requirements

### Hardware

- **Seeed Studio XIAO ESP32-C3**
- **128x64 OLED Display (I2C)**
- External LED connected to pin `D10` (optional for debugging).

### Software

- Arduino IDE with the following libraries installed:
  - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
  - [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
  - [ArduinoJson](https://arduinojson.org/)
  - [ESP32 Board Support Package](https://github.com/espressif/arduino-esp32)

## Setup

1. Clone this repository:
   ```bash
   git clone https://github.com/srijan101/XIAOESP32C3_OLED_ThingSpeak.git
