# ESP32 Network Scanner

A Wi-Fi and Bluetooth device scanner using ESP32 C3.

## 🌟 Features

- Wi-Fi Network Scanning:
  - SSID detection
  - Signal strength (RSSI) measurement
  - Encryption type identification
  - Channel information
  - BSSID detection
- Bluetooth Device Scanning:
  - Device name detection
  - MAC address identification
  - Signal strength measurement
  - Vendor information
- JSON export for Wi-Fi and Bluetooth scan results

## 📋 Requirements

### Hardware
- ESP32 C3 development board
- USB cable for programming

### Software
- Arduino IDE![2](https://github.com/user-attachments/assets/4ebbd986-e6cf-4603-aeff-e13d26ccf17c)

- Required libraries:
  ```
  - WiFi.h
  - WebServer.h
  - ArduinoJson.h
  - BLEDevice.h
  - BLEScan.h
  - BLEAdvertisedDevice.h
  ```

## 📱 Usage

1. Power up the ESP32
2. Connect to the ESP32's Wi-Fi access point
3. Open a Chromium-based browser and navigate to `192.168.4.1`
   - Note: Safari is not supported
4. Log in using the configured password
5. Use tabs to switch between Wi-Fi and Bluetooth scanning
6. Click "Scan" to start network discovery
7. Use "Save" to export results

## ⚠️ Security Notes

- Change default credentials before use
- Passwords stored in plaintext

## 📄 License
Copyright 2024 Leucophyllous (Leila_9136)
Licensed under the Apache License, Version 2.0 (the "License");
you may freely use this software except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
