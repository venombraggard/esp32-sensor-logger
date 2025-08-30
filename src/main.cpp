#include <Arduino.h>
#include <FS.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPIFFS.h>

// Digital pin connected to the DHT sensor 
#define DHTPIN 5     
#define DHTTYPE DHT11   // DHT 11

// DHT object
DHT dht(DHTPIN, DHTTYPE);

// File object for SPIFFS
File fichier;

// Global timers
unsigned long previousTimeTwoS = 0;
unsigned long previousTimeOneS = 0;

// Check 2s interval (non-blocking)
bool intervalTwoS() {
  unsigned long currentTime = millis();
  if (currentTime - previousTimeTwoS >= 2000) {
    previousTimeTwoS = currentTime;
    return true;
  }
  return false;
}

// Check 1s interval (non-blocking)
bool intervalOneS() {
  unsigned long currentTime = millis();
  if (currentTime - previousTimeOneS >= 1000) {
    previousTimeOneS = currentTime;
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);

  // Init DHT sensor
  dht.begin();

  // Init SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error initializing SPIFFS...");
    return;
  }

  // Open file in append mode (create if not exists)
  fichier = SPIFFS.open("/export.csv", FILE_APPEND);
  if (!fichier) {
    Serial.println("Error opening CSV file...");
    return;
  }

  // Write header if file is empty
  if (fichier.size() == 0) {
    fichier.println("timestamp,temperature");
    fichier.flush();
  }
}

void loop() {
  // Every 2 seconds
  if (intervalTwoS()) {
    float temp = dht.readTemperature();

    // Check if reading failed
    if (isnan(temp)) {
      Serial.println("Failed to read DHT11 sensor...");
      return;
    }

    // Current timestamp in seconds
    unsigned long now = millis() / 1000;

    // Write to CSV
    fichier.printf("%lu, %.2f\n", now, temp);
    fichier.flush(); // Ensure data is written to SPIFFS

    // Print to Serial Monitor
    Serial.printf("t=%lu s, Temp: %.2f °C\n", now, temp);
  }
}
