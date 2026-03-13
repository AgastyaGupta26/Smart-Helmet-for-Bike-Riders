# Smart Helmet for Bike Riders

An IoT-based safety system for motorcycle riders that detects accidents and alcohol consumption and automatically alerts the vehicle system to stop the engine.

This project was developed using ESP32 microcontrollers, sensors, and wireless communication to improve rider safety.

## 🚀 Features

- Detects bike crashes using MPU6050 accelerometer
- Detects alcohol consumption using MQ-3 sensor
- Wireless communication via WiFi (UDP protocol)
- Automatic engine stop simulation using servo motor
- Real-time alerts displayed on LCD
- Buzzer alert system
- LED indicators for crash and alcohol alerts
- Automatic engine resume after emergency timeout

## 🧠 System Architecture

The system consists of two modules:

1. **Helmet Module (Sender)**
   - Installed inside the rider's helmet.
   - Detect alcohol from rider breath.
   - Detect crashes or heavy shaking.
   - Send alert message to vehicle system.
   - Sensors used:
     - MPU6050 → Crash detection
     - MQ-3 Gas Sensor → Alcohol detection

2. **Vehicle Module (Receiver)**
   - Installed on the bike/vehicle.
   - Receive alerts from helmet.
   - Stop engine (simulated using servo motor).
   - Display alerts on LCD.
   - Activate buzzer and LEDs.
   - Components used:
     - ESP32
     - Servo Motor
     - I2C LCD Display
     - Buzzer
     - LED Indicators

## 📡 Communication

The helmet ESP32 connects to the vehicle ESP32 via WiFi hotspot.

Protocol: UDP (User Datagram Protocol)

Network configuration:

- SSID: HELMET_ALERT_SYSTEM
- Password: stopnow123
- Port: 12345
- Receiver IP: 192.168.4.1

## 🧰 Hardware Components

| Component | Quantity |
| --- | --- |
| ESP32 Development Board | 2 |
| MPU6050 Accelerometer | 1 |
| MQ3 Alcohol Sensor | 1 |
| Servo Motor | 1 |
| 16x2 I2C LCD Display | 1 |
| Buzzer | 1 |
| LEDs | 2 |
| Resistors | As required |
| Breadboard | 1 |
| Jumper Wires | Multiple |

## ⚙️ Working Principle

### Normal Operation

Helmet ESP32 continuously monitors:
- Acceleration values
- Alcohol sensor readings

Vehicle ESP32 keeps the engine running (servo movement).
LCD shows:
- SYSTEM ACTIVE
- ENGINE RUNNING

### Alcohol Detection

If alcohol level exceeds threshold:
- ALCOHOL_THRESHOLD = 2100
Actions triggered:
- Alert sent to vehicle
- Engine stops
- LCD shows alert
- Buzzer beeps
- Alcohol LED lights up

LCD display:
- ALCOHOL ALERT!
- ENGINE LOCKED

### Crash Detection

Crash is detected using total acceleration magnitude.
If acceleration exceeds threshold:
- SHAKE_THRESHOLD = 25.0
Actions triggered:
- Alert sent to vehicle
- Engine stops
- Crash LED turns on
- Buzzer beeps
- LCD shows crash message

LCD display:
- CRASH DETECTED!
- STOPPING...

## 🔧 Software Libraries Used

Install the following Arduino libraries before compiling:

- Adafruit MPU6050
- Adafruit Unified Sensor
- ESP32Servo
- LiquidCrystal_I2C
- WiFi
- WiFiUdp
- Wire

## 💻 Installation

1. Clone Repository

```
git clone https://github.com/AgastyaGupta26/smart-helmet-system.git
```

2. Open in Arduino IDE

Open these files:
- sender_helmet/sender_helmet.ino
- receiver_vehicle/receiver_vehicle.ino

3. Upload Code

Upload to two ESP32 boards:

| Board | Code |
|---|---|
| Helmet ESP32 | sender_helmet.ino |
| Vehicle ESP32 | receiver_vehicle.ino |

## 📁 Project Structure

```
smart-helmet-system
│
├── sender_helmet
│   └── sender_helmet.ino
│
├── receiver_vehicle
│   └── receiver_vehicle.ino
│
├── images
│   └── prototype.jpg
│
└── README.md
```

## 🖼️ Future Improvements

Possible upgrades:
- GPS accident location tracking
- GSM module for emergency SMS
- Mobile application integration
- Helmet wearing detection
- Cloud-based monitoring

## 👨‍💻 Author

Agastya Gupta

B.Tech Electronics and Communication Engineering

## 📜 License

This project is open source and available under the MIT License.

⭐ If you like this project, consider giving it a star on GitHub! 
