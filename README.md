# Smart Helmet For Bike Riders

A two-module IoT safety system for bike riders.

- **Sender Helmet** (Module A): ESP32-based sensor pack that detects crashes and alcohol level with MPU-6050 and MQ-3, then sends UDP alerts.
- **Receiver Helmet** (Module B): ESP32-based receiver that listens for UDP alerts and displays warnings.

## Features

- Shake/crash detection via MPU-6050 (accelerometer + gyroscope)
- Alcohol detection via MQ-3 analog sensor
- Wi-Fi UDP alert transport
- Visual/serial feedback on board LED and Serial Monitor
- Alert cooldown logic to avoid floods (5 seconds)

## Folder Structure

- `Sender Helmet/`
  - `sender_helmet.ino`
- `Receiver Helmet/`
  - `receiver_vehicle.ino` (based on your editor context)

## Prerequisites

- Arduino IDE (or PlatformIO)
- ESP32 board support installed (e.g. `ESP32 Dev Module`)
- Libraries:
  - `Adafruit MPU6050`
  - `Adafruit Unified Sensor`
  - `Wire` (core)
  - `WiFi` (ESP32 core)

## Hardware

### Sender Helmet

- ESP32 (or compatible)
- MPU-6050:
  - `SDA` -> GPIO 21
  - `SCL` -> GPIO 22
  - `VCC` -> 3.3V
  - `GND` -> GND
- MQ-3:
  - `A0` -> GPIO 34
  - `VCC` -> 5V (or 3.3V per module spec)
  - `GND` -> GND
- LED on `GPIO 2` (built-in)

### Receiver Helmet

- ESP32 (or compatible)
- (Add display/buzzer if required by your receiver code)

## Configuration

In `Sender Helmet/sender_helmet.ino`:

- SSID: `HELMET_ALERT_SYSTEM`
- Password: `stopnow123`
- Receiver IP: `192.168.4.1`
- UDP Port: `12345`
- Alcohol threshold: `2100`
- Shake threshold: `25.0`

Adjust as needed.

## Upload Steps

1. Open `sender_helmet.ino` in Arduino IDE.
2. Set board to `ESP32 Dev Module`.
3. Select correct COM port.
4. Verify then Upload.
5. Repeat for `receiver_vehicle.ino` in `Receiver Helmet`.

## Debug

- Use Serial Monitor at `115200`.
- Ensure both devices are on same Wi-Fi network (or AP/Station pair).
- If header errors (`Wire.h`, `Adafruit_MPU6050.h`, `WiFi.h`) appear, check board type + install missing libraries.

## Usage

- Wear/attach Sender sensors.
- Trigger a shake threshold or alcohol event.
- Sender sends alert packet to receiver via UDP.
- Receiver reports alert (Serial/UI).

## Notes

- `receiverIP` can be changed to dynamic host if using existing AP with DHCP.
- Cooldown prevents multiple alerts in rapid sequence.

---
