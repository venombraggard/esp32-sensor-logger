# ESP32 Sensor Logger

A simple project to log sensor data on the ESP32.  
The project starts with simulated sensor values and then extends to real hardware (DHT11, ESP32, HTTP server, etc.).

## Features

- ✅ Simulated temperature sensor (random values)
- ✅ Data logging to `.csv` file (with timestamp)
- ⏳ Real sensor support (DHT11 on ESP32)
- ⏳ Local storage on ESP32 (SPIFFS)
- ⏳ Data transmission to HTTP server

## Project Structure
.
├── include/ # Header files
├── lib/ # External libraries
├── src/ # Main source code
├── test/ # Unit tests
├── platformio.ini # PlatformIO configuration
└── README.md


## Getting Started

### Requirements
- [PlatformIO](https://platformio.org/)  
- ESP32 board  
- (Optional) DHT11 sensor  

### Build & Upload
```bash
pio run
pio run --target upload
pio device monitor
``` 

## Roadmap

 Sensor simulation with random values
 CSV data logging with timestamps
 Read temperature from DHT11
 Store data in ESP32 SPIFFS
 Send data to remote HTTP server
