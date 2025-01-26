# ESP32 Network Scanner

An ESP32-based web application that scans and displays nearby Wi-Fi networks and Bluetooth devices. Features a secure login system and data export capabilities.

## Features

- Web-based interface with password protection
- Wi-Fi network scanning with detailed information:
  - SSID
  - Signal strength (RSSI)
  - Encryption type
  - Channel
  - BSSID
- Bluetooth device scanning with:
  - Device name
  - MAC address
  - Signal strength
  - Manufacturer data
- JSON data export for both Wi-Fi and Bluetooth scan results
- Responsive design with tabbed interface

## Hardware Requirements

- ESP32 development board
- USB cable for programming

## Software Requirements

- Arduino IDE
- Required libraries:
  - WiFi.h
  - WebServer.h
  - ArduinoJson.h
  - BLEDevice.h
  - BLEScan.h
  - BLEAdvertisedDevice.h

## Setup Instructions

1. Clone this repository
2. Open the project in Arduino IDE
3. Install required libraries through Arduino Library Manager
4. Configure the following in the code:
   - Set `AP_SSID` to your desired access point name
   - Set `AP_PASSWORD` to your desired AP password
   - Set the login password in the `LOGIN_HTML` section
5. Upload the code to your ESP32

## Usage

1. Power on your ESP32
2. Connect to the ESP32's Wi-Fi access point (SSID configured in setup)
3. Navigate to `192.168.4.1` in your web browser
4. Log in using the configured password
5. Use the tabs to switch between Wi-Fi and Bluetooth scanning
6. Click scan buttons to start network discovery
7. Export results using the save buttons

## Security Notes

- Default credentials should be changed before deployment
- All passwords are stored in plaintext - use for educational purposes only
- Login system uses client-side validation

## License

MIT License

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
