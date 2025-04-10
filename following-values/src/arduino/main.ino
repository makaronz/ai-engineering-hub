#include <Wire.h>
#include <AS7262.h>
#include <Adafruit_TSL2591.h>
#include <WiFi.h>
#include <GSM.h>
#include <ArduinoJson.h>
#include <RTCZero.h>

// Pin Definitions
#define LED_PIN 13
#define BATTERY_PIN A0

// Sensor Objects
AS7262 spectralSensor;
Adafruit_TSL2591 luminanceSensor = Adafruit_TSL2591(2591);
RTCZero rtc;

// WiFi Credentials
const char* ssid = "HaLow_Network";
const char* password = "your_password";

// GSM Configuration
const char* apn = "your_apn";
const char* gprsUser = "your_username";
const char* gprsPass = "your_password";

// Global Variables
bool useGSM = false;
unsigned long lastTransmission = 0;
const unsigned long transmissionInterval = 1000; // 1 second

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  while (!Serial) delay(10);

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize I2C
  Wire.begin();

  // Initialize Sensors
  setupSpectralSensor();
  setupLuminanceSensor();
  
  // Initialize RTC
  rtc.begin();
  
  // Initialize Communication
  setupCommunication();
}

void loop() {
  // Read Sensors
  SensorData data = readSensors();
  
  // Check Battery
  data.batteryLevel = readBatteryLevel();
  
  // Transmit Data
  if (millis() - lastTransmission >= transmissionInterval) {
    transmitData(data);
    lastTransmission = millis();
  }
  
  // Check Communication Status
  checkCommunicationStatus();
  
  delay(100);
}

void setupSpectralSensor() {
  if (!spectralSensor.begin()) {
    Serial.println("Failed to initialize AS7262!");
    while (1);
  }
  
  // Configure AS7262
  spectralSensor.setIntegrationTime(50);
  spectralSensor.setGain(1);
  spectralSensor.setMeasurementMode(AS7262_MEASUREMENT_MODE_6CHAN_ONE_SHOT);
}

void setupLuminanceSensor() {
  if (!luminanceSensor.begin()) {
    Serial.println("Failed to initialize TSL2591!");
    while (1);
  }
  
  // Configure TSL2591
  luminanceSensor.setGain(TSL2591_GAIN_MED);
  luminanceSensor.setTiming(TSL2591_INTEGRATIONTIME_300MS);
}

void setupCommunication() {
  // Try HaLow first
  WiFi.begin(ssid, password);
  
  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    attempts++;
  }
  
  // If HaLow fails, switch to GSM
  if (WiFi.status() != WL_CONNECTED) {
    useGSM = true;
    setupGSM();
  }
}

void setupGSM() {
  GSM.begin();
  GSM.attachGPRS(apn, gprsUser, gprsPass);
}

SensorData readSensors() {
  SensorData data;
  
  // Read Spectral Data
  spectralSensor.takeMeasurements();
  data.spectralData[0] = spectralSensor.getCalibratedViolet();
  data.spectralData[1] = spectralSensor.getCalibratedBlue();
  data.spectralData[2] = spectralSensor.getCalibratedGreen();
  data.spectralData[3] = spectralSensor.getCalibratedYellow();
  data.spectralData[4] = spectralSensor.getCalibratedOrange();
  data.spectralData[5] = spectralSensor.getCalibratedRed();
  
  // Read Luminance Data
  uint32_t lum = luminanceSensor.getFullLuminosity();
  data.luminance = lum;
  
  // Get Timestamp
  data.timestamp = rtc.getEpoch();
  
  return data;
}

float readBatteryLevel() {
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue * 3.3) / 1024.0;
  return (voltage - 3.0) / (4.2 - 3.0) * 100.0; // Convert to percentage
}

void transmitData(SensorData data) {
  // Create JSON document
  StaticJsonDocument<512> doc;
  
  // Add spectral data
  JsonArray spectral = doc.createNestedArray("spectral");
  for (int i = 0; i < 6; i++) {
    spectral.add(data.spectralData[i]);
  }
  
  // Add other data
  doc["luminance"] = data.luminance;
  doc["battery"] = data.batteryLevel;
  doc["timestamp"] = data.timestamp;
  
  // Serialize JSON
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Transmit data
  if (useGSM) {
    transmitViaGSM(jsonString);
  } else {
    transmitViaHaLow(jsonString);
  }
}

void transmitViaHaLow(String data) {
  // Implement HaLow transmission
  // This is a placeholder - actual implementation depends on HaLow module
}

void transmitViaGSM(String data) {
  // Implement GSM transmission
  // This is a placeholder - actual implementation depends on GSM module
}

void checkCommunicationStatus() {
  if (!useGSM) {
    if (WiFi.status() != WL_CONNECTED) {
      // Try to reconnect
      WiFi.begin(ssid, password);
      delay(1000);
      
      // If still not connected, switch to GSM
      if (WiFi.status() != WL_CONNECTED) {
        useGSM = true;
        setupGSM();
      }
    }
  } else {
    // Check GSM connection
    if (!GSM.isGPRSConnected()) {
      GSM.attachGPRS(apn, gprsUser, gprsPass);
    }
  }
}

// Data Structure
struct SensorData {
  float spectralData[6];
  uint32_t luminance;
  float batteryLevel;
  unsigned long timestamp;
}; 