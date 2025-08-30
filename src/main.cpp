#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousTimeTwoS = 0;

// =========================
// SPIFFS Functions
void listFiles() {
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.printf("File: %s, size: %d\n", file.name(), file.size());
    file = root.openNextFile();
  }
}

void readFile(const char *path) {
  File file = SPIFFS.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("Failed to open file");
    return;
  }
  Serial.printf("=== Content of %s ===\n", path);
  while (file.available()) {
    Serial.write(file.read());
  }
  Serial.println("\n=======================");
  file.close();
}
//


// =========================
// Millis Timer
bool intervalTwoS() {
  unsigned long currentTime = millis();
  if (currentTime - previousTimeTwoS >= 2000) {
    previousTimeTwoS = currentTime;
    return true;
  }
  return false;
}
//

// =========================
// Setup 
void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!SPIFFS.begin(true)) {
    Serial.println("Error initializing SPIFFS...");
    return;
  }

  // Header of CSV file
  File fichier = SPIFFS.open("/data.csv", FILE_APPEND);
  if (!fichier) {
    Serial.println("Error opening CSV file...");
    return;
  }
  if (fichier.size() == 0) {
    fichier.println("timestamp,temperature");
    fichier.flush();
  }
  fichier.close();

  // Printing existing files
  listFiles();
  readFile("/data.csv");
}

void printLastLines(const char* path, int linesToShow) {
  File file = SPIFFS.open(path);
  if (!file) return;

  // Read
  String content;
  while (file.available()) {
    content += (char)file.read();
  }
  file.close();

  // Découper en lignes
  int count = 0;
  int idx = content.length() - 1;
  while (idx >= 0 && count < linesToShow) {
    if (content[idx] == '\n') count++;
    idx--;
  }

  Serial.println("=== Last measurements ===");
  Serial.println(content.substring(idx + 2)); // +2 
  Serial.println("========================");
}


void loop() {
  if (intervalTwoS()) {
    float temp = dht.readTemperature();
    if (isnan(temp)) {
      Serial.println("Failed to read DHT11 sensor...");
      return;
    }

    unsigned long now = millis() / 1000;

    // Writing in CSV
    File fichier = SPIFFS.open("/data.csv", FILE_APPEND);
    if (fichier) {
      fichier.printf("%lu,%.2f\n", now, temp);
      fichier.close();
    } else {
      Serial.println("Erreur ouverture CSV");
    }

    // Read only last value
    Serial.printf("t=%lu s, Temp: %.2f °C\n", now, temp);

    // Afficher les 10 dernières lignes du CSV
    printLastLines("/data.csv", 5);
  }
}

