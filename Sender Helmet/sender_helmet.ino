/*
 * =============================================================
 * PROJECT: SMART HELMET (MODULE A - SENDER)
 * * WHAT IT DOES:
 * 1. Reads MPU-6050 for Shake Detection.
 * 2. Reads MQ-3 for Alcohol Detection (Threshold: 2100).
 * 3. Connects to Receiver's Wi-Fi.
 * 4. Sends alerts via UDP.
 * =============================================================
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// --- MPU-6050 Settings (Shake) ---
Adafruit_MPU6050 mpu;
const float SHAKE_THRESHOLD = 25.0; 
const int LED_PIN = 2;                  

// --- MQ-3 Settings (Alcohol) ---
const int MQ3_PIN = 34; // Analog Pin 34

// *** UPDATED THRESHOLD ***
const int ALCOHOL_THRESHOLD = 2100; 

// --- Cooldown Timers ---
unsigned long lastTriggerTime = 0;
const long alertCooldown = 5000; // 5 seconds between alerts

// --- Wi-Fi & UDP Settings ---
const char *ssid = "HELMET_ALERT_SYSTEM"; 
const char *password = "stopnow123";      
WiFiUDP udp;
IPAddress receiverIP(192, 168, 4, 1);
unsigned int udpPort = 12345; 

// --- Function Declaration ---
void sendAlert(String message);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MQ3_PIN, INPUT); 
  digitalWrite(LED_PIN, LOW);

  // --- MPU-6050 Setup ---
  Serial.println("Initializing sensors...");
  // The code will wait here if your MPU-6050 wiring is loose!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip! CHECK WIRING (Pins 21 & 22)");
    while (1) { 
      digitalWrite(LED_PIN, HIGH); delay(100);
      digitalWrite(LED_PIN, LOW); delay(100);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.println("MPU-6050 Ready.");

  // --- Wi-Fi Connection ---
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  
  Serial.println("Sensors warming up...");
  delay(2000);
}

void loop() {
  unsigned long currentTime = millis();

  // --- 1. READ ALCOHOL SENSOR ---
  int alcoholValue = analogRead(MQ3_PIN);
  
  // Print value to Serial Monitor to verify
  static unsigned long lastPrint = 0;
  if (currentTime - lastPrint > 500) {
    Serial.print("Alcohol Reading: ");
    Serial.println(alcoholValue);
    lastPrint = currentTime;
  }

  // Check for Alcohol
  // Must be > 2100 AND passed the 5 second cooldown
  if (alcoholValue > ALCOHOL_THRESHOLD && (currentTime - lastTriggerTime > 5000)) {
    sendAlert("ALCOHOL DETECTED! DRUNK RIDER!");
    lastTriggerTime = currentTime;
  }

  // --- 2. READ SHAKE SENSOR ---
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float totalAccel = sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2));

  // Check for Shake
  if (totalAccel > SHAKE_THRESHOLD && (currentTime - lastTriggerTime > alertCooldown)) {
    sendAlert("CRASH DETECTED! STOP VEHICLE!");
    lastTriggerTime = currentTime;
  }

  delay(100); 
}

// --- HELPER FUNCTION TO SEND MESSAGES ---
void sendAlert(String message) {
  Serial.println("\n***************************");
  Serial.println("!!! SENDING ALERT !!!");
  Serial.println(message);
  Serial.println("***************************");

  udp.beginPacket(receiverIP, udpPort);
  udp.print(message);
  udp.endPacket();

  // Blink LED fast
  for(int i=0; i<5; i++) {
    digitalWrite(LED_PIN, HIGH); delay(100);
    digitalWrite(LED_PIN, LOW); delay(100);
  }
}