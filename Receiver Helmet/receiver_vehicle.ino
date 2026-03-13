/*
 * =============================================================
 * PROJECT: SMART HELMET (MODULE B - RECEIVER) - FINAL COMPLETE
 * * WHAT IT DOES:
 * 1. Creates Wi-Fi Hotspot.
 * 2. Normal: Servo moves, LCD shows "ENGINE RUNNING".
 * 3. Alert: Servo STOPS, LED ON, Beeps, LCD shows ALERT TYPE.
 * 4. Auto-Resumes engine after 5 seconds.
 * =============================================================
*/

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

// --- Wi-Fi Settings ---
const char *ssid = "HELMET_ALERT_SYSTEM";
const char *password = "stopnow123";
WiFiUDP udp;
unsigned int udpPort = 12345;
char packetBuffer[255]; 

// --- PIN DEFINITIONS ---
// DOUBLE CHECK YOUR WIRING AGAINST THESE PINS!
const int SERVO_PIN = 13;       // Orange wire
const int LED_CRASH_PIN = 26;   // Red LED +
const int LED_ALCOHOL_PIN = 27; // Blue/Yellow LED +
const int BUZZER_PIN = 14;      // Buzzer Long Leg +

// --- OBJECTS ---
Servo vehicleServo;
// IMPORTANT: Ensure this address (0x27 or 0x3F) matches what worked for you!
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- Servo Variables ---
int servoPos = 0;
bool movingForward = true;

// --- Function Declarations ---
void handleEmergency(String line1, String line2);
void moveServo();

void setup() {
  Serial.begin(115200);
  
  // 1. Setup Pins
  pinMode(LED_CRASH_PIN, OUTPUT);
  pinMode(LED_ALCOHOL_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_CRASH_PIN, LOW);
  digitalWrite(LED_ALCOHOL_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // 2. Setup Servo
  vehicleServo.setPeriodHertz(50); 
  vehicleServo.attach(SERVO_PIN, 500, 2400);

  // 3. Setup LCD (The part you just fixed!)
  Wire.begin(21, 22); // Explicitly set SDA=21, SCL=22
  lcd.init();      
  lcd.backlight(); 
  
  lcd.setCursor(0, 0);
  lcd.print("RECEIVER SYSTEM");
  lcd.setCursor(0, 1);
  lcd.print("BOOTING UP...");
  delay(2000);

  // 4. Setup Wi-Fi
  Serial.println("Receiver Booting...");
  WiFi.softAP(ssid, password);
  udp.begin(udpPort);
  Serial.println("VEHICLE STARTED.");

  // 5. Show Normal State on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM ACTIVE");
  lcd.setCursor(0, 1);
  lcd.print("ENGINE RUNNING");
}

void loop() {
  // --- 1. CHECK FOR INCOMING SIGNALS ---
  int packetSize = udp.parsePacket();
  
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    String message = String(packetBuffer);

    Serial.print("\nALERT RECEIVED: ");
    Serial.println(message);

    // --- 2. HANDLE ALERT ---
    if (message.indexOf("ALCOHOL") >= 0) {
      digitalWrite(LED_ALCOHOL_PIN, HIGH);
      // Show alcohol alert on screen
      handleEmergency("ALCOHOL ALERT!", "ENGINE LOCKED");
      digitalWrite(LED_ALCOHOL_PIN, LOW);
    } 
    else if (message.indexOf("CRASH") >= 0) {
      digitalWrite(LED_CRASH_PIN, HIGH);
      // Show crash alert on screen
      handleEmergency("CRASH DETECTED!", "STOPPING...");
      digitalWrite(LED_CRASH_PIN, LOW);
    }
    
    // Reset to Normal Screen after alert is cleared
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM ACTIVE");
    lcd.setCursor(0, 1);
    lcd.print("ENGINE RUNNING");
  }

  // --- 3. NORMAL OPERATION ---
  // If no alert is happening, keep the engine (servo) moving
  moveServo(); 
}

// --- HELPER FUNCTIONS ---

// This function handles the screen text, buzzer, and 5-second pause
void handleEmergency(String line1, String line2) {
  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);

  // Beep 5 times fast
  for(int i=0; i<5; i++){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }

  // Wait 3.5 seconds (Total stop time is approx 5 seconds including beeps)
  // The servo stops moving here because moveServo() is not being called.
  delay(3500); 
  
  // Clear old packets so it doesn't trigger again instantly
  udp.flush(); 
}

// Function to sweep the servo back and forth
void moveServo() {
  if (movingForward) {
    servoPos += 3; 
    if (servoPos >= 160) movingForward = false;
  } else {
    servoPos -= 3;
    if (servoPos <= 20) movingForward = true;
  }
  vehicleServo.write(servoPos);
  delay(15); 
}