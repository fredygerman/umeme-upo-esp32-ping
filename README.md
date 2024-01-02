# Power Availability Monitor

## Overview

This project involves an ESP-based device that pings a server to keep track of power availability at a specific location. The device is programmed to send periodic requests to the server, and it can also report errors if any occur during the process.

## Getting Started

### Prerequisites

- Arduino IDE installed
- ESP board support installed in Arduino IDE
- Necessary libraries: WiFi, HTTPClient, ArduinoJson

  To install the ArduinoJson library:

  1. Open Arduino IDE
  2. Navigate to **Sketch -> Include Library -> Manage Libraries...**
  3. In the Library Manager, type "ArduinoJson" in the search bar.
  4. Click on "ArduinoJson" by Benoit Blanchon and click "Install."

### Installation

1. Open the project in the Arduino IDE.
2. Update the following constants in the code:

   - `ssid`: Your WiFi SSID.
   - `password`: Your WiFi password.
   - `serverAddress`: Your server's address.
   - `apikey`: Your authorization API key from Supabase.
   - `location`: Your location.
   - `errorReportingAddress`: Your error reporting address.

3. Upload the code to your ESP device.

## Usage

- The device will attempt to connect to the specified WiFi network.
- Once connected, it will start sending GET requests to the server.
- If a successful response (HTTP 204) is received, an LED on the device will blink.

  After a successful run, you can check previous error logs by accessing the serial monitor.

- If an error occurs, it will be logged and can be reported to an error reporting address.

## Error Reporting

- Error reporting is enabled by default and can be disabled by setting `enableErrorReporting` to `false`.
- If errors occur during GET requests, they are logged.
- Periodically, the device will attempt to report these errors to the specified error reporting address.

## Configuration

- Adjust the delay time between requests (`delayTime`) and on error delay time (`onErrorDelayTime`) as needed.
- Set `enableErrorReporting` to `false` to disable error reporting.

## Known Issues 🚧

- The device may fail to connect to WiFi if the network is not available.

#### If the LED is on for more than 5 seconds, an error has occurred. The first thing to do is to check your wifi connectivity and UNPLUG and PLUG the device back in. If the LED is still on, check the serial monitor for detailed information on errors.

#### For any other issues, please open an [issue on GitHub.](https://github.com/fredygerman/umeme-upo-esp32-ping/issues)

## Troubleshooting

- All errors will cause the LED to blink to stay on. So if the LED is on for more than 5 seconds, an error has occurred.
- If the device fails to connect to WiFi, the LED will remain on.
- Check the serial monitor for detailed information on errors.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
