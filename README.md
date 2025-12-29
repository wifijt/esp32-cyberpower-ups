# ESPHome - ESP32 CyberPower UPS Monitor

An ESPHome custom component for the ESP32-S3 that monitors CyberPower UPS units via USB HID. This driver allows you to bring real-time power data into Home Assistant without the need for a dedicated NUT server.

## Features

- **Native USB HID**: Directly communicates with the UPS via the ESP32-S3 USB Host hardware.
- **Real-time Sensors**: Reports Output Watts, VA, Battery Percentage, and Load.
- **Power Status**: Binary sensor for "Mains (AC)" vs "Battery Backup".
- **Dynamic Runtime Estimation**: User-side calculation that accounts for battery capacity and discharge percentage.
- **Dashboard Integration**: Includes project metadata for seamless display in the ESPHome dashboard.

## Hardware Requirements

This was tested with the Freenove ESP32-S3 8MB Flash with the USB-OTG port, like this one https://amzn.to/4shbEI1

You will also need a USB-A to USB-C cable like this https://amzn.to/3Yfwaet

The UPS it was tested against was a CyberPower 1350VA from Costco

## Tested versions

This was built on ESPHome 2025.12.0 - but may work with earlier versions
It requires ESP-IDF (5.5.1) and several build options as shown in the example YAML to ensure that the USB-OTG port isn't configured for serial logging

```yaml
esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: esp-idf
    version: 5.5.1
    sdkconfig_options:
        CONFIG_USB_OTG_SUPPORTED: "y"
        CONFIG_USB_HOST_CONTROL_TRANSFER_MAX_SIZE: "256"
        CONFIG_ESP_CONSOLE_UART_DEFAULT: "y"
        CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG: "n"

