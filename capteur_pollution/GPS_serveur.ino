#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <WiFiS3.h>
#include <SD.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>

// GPS via SoftwareSerial
SoftwareSerial ss(4, 3); // RX (Arduino) = 4, TX (Arduino) = 3
TinyGPSPlus gps;

// Wi-Fi
const char* ssid = "SamuelB";
const char* password = "Samuelbg";
const char* serverUrl = "http://192.168.1.50/upload";

WiFiClient client;
HttpClient http(client, "192.168.1.50", 80);

// SD
#define SD_CS_PIN 10
File logFile;

// Timing
unsigned long lastGPSTime = 0;
const unsigned long gpsInterval = 20000;
bool wifiConnected = false;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  delay(1000);
  Serial.println("helloword");
  // Init SD
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Erreur carte SD !");
    while (0);
  }

  // Connexion Wi-Fi (1 tentative)
  WiFi.begin(ssid, password);
  Serial.print("Connexion Wi-Fi...");
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 5000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnecté !");
    wifiConnected = true;
  } else {
    Serial.println("\nWi-Fi NON connecté.");
  }
  
}

void loop() {
  while (ss.available()) {
    gps.encode(ss.read());
  }

  if (millis() - lastGPSTime > gpsInterval) {
    lastGPSTime = millis();

    if (1 || gps.location.isUpdated()) {
      String ligne = String(gps.date.day()) + "," + String(gps.date.month()) + "," + String(gps.date.year()) + "," +
                     String(gps.time.hour()) + "," + String(gps.time.minute()) + "," + String(gps.time.second()) + "," +
                     String(gps.speed.kmph()) + "," + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
      Serial.println(ligne);

      logFile = SD.open("data.csv", FILE_WRITE);
      if (logFile) {
        logFile.println(ligne);
        logFile.close();
      }

      if (wifiConnected) {
        sendFile("data.csv");
      }
    } else {
      Serial.println("Fix GPS non disponible.");
    }
  }
}

void sendFile(const char* filename) {
  File file = SD.open(filename);
  if (!file) {
    Serial.println("Erreur ouverture fichier.");
    return;
  }

  String content;
  while (file.available()) {
    content += (char)file.read();
  }
  file.close();

  http.beginRequest();
  http.post("/upload");
  http.sendHeader("Content-Type", "text/plain");
  http.sendHeader("Content-Length", content.length());
  http.beginBody();
  http.print(content);
  http.endRequest();

  int statusCode = http.responseStatusCode();
  String response = http.responseBody();
  Serial.print("Serveur : ");
  Serial.println(statusCode);
  Serial.println(response);
}
