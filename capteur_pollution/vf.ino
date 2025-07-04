#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <WiFiS3.h>
#include <SdFat.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>

// GPS via SoftwareSerial
//SoftwareSerial Serial(0, 1); // RX (Arduino) = 0, TX (Arduino) = 1
TinyGPSPlus gps;

// Wi-Fi
const char* Serialid = "Samuel";
const char* paSerialword = "Sam";
const char* serverUrl = "http://192.168.1.50/upload";

WiFiClient client;
HttpClient http(client, "192.168.1.50", 80);

// SD
//#define SD_CS_PIN 10

SdFat sd;
FsFile fichier;



SoftwareSerial mySerial(D2, D7);

const int chipSelect = 10;

// Timing
unsigned long lastGPSTime = 0;
const unsigned long gpsInterval = 2000;
bool wifiConnected = false;
bool valid_reading = false;
uint8_t buffer[32];
uint16_t pm1_0 = 0, pm2_5 = 0, pm10 = 0;
int buff_index = 0;

void setup() {
  Serial.begin(9600);
  //ss.begin(9600);
  mySerial.begin(9600);
  delay(1000);
  //Serial.println("helloword");

  // Initialisation de la carte SD
  if (!sd.begin(chipSelect, SD_SCK_MHZ(10))) {
    //Serial.println("Échec de l'initialisation !");
    return;
  } else {
    //Serial.println("Connexion SD OK !");
  }

  // Écriture de l’en-tête du fichier CSV
  fichier = sd.open("data.csv", FILE_WRITE);
  if (fichier) {
    fichier.println("#D, M, Y, h, min, sec, v, lat, long,PM1.0,PM2.5,PM10");
    fichier.close();
  }

  // Connexion Wi-Fi (1 tentative)
  WiFi.begin(Serialid, paSerialword);
  //Serial.print("Connexion Wi-Fi...");
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 5000) {
    delay(500);
    //Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    //Serial.println("\nConnecté !");
    wifiConnected = true;
  } else {
    //Serial.println("\nWi-Fi NON connecté.");
  }
  
}

void loop() {
  
  while (Serial.available()) {
    gps.encode(Serial.read());
  }

  if (millis() - lastGPSTime > gpsInterval) {
    lastGPSTime = millis();
   
    lireCapteur();
   
    delay(100);
    if (valid_reading) ecrireCSV(pm1_0, pm2_5, pm10);

  }
  if (wifiConnected) {
        sendFile("data.csv");
      }
}

void ecrireCSV(uint16_t pm1_0, uint16_t pm2_5, uint16_t pm10) {
  fichier = sd.open("data.csv", FILE_WRITE);
  if (1 || gps.location.isUpdated()) {
      String ligne = String(gps.date.day()) + "," + String(gps.date.month()) + "," + String(gps.date.year()) + "," +
                     String(gps.time.hour()) + "," + String(gps.time.minute()) + "," + String(gps.time.second()) + "," +
                     String(gps.speed.kmph()) + "," + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
      //Serial.println(ligne);
    if (fichier) {
    fichier.print(ligne); fichier.print(",");
    fichier.print(pm1_0); fichier.print(",");
    fichier.print(pm2_5); fichier.print(",");
    fichier.println(pm10);
    fichier.close();
  }
  } else {
      //Serial.println("Fix GPS non disponible.");
  }
}


void lireCapteur() {
  while (mySerial.available()) mySerial.read();
  delay(1000);

  int i = 0;
  //Serial.print("debug pms : ");
  //if (!mySerial.available()){Serial.println("not available");}
  while (mySerial.available() && i < 32) {

    buffer[i++] = mySerial.read();
    //Serial.println(buffer[i-1]);
  }
  //Serial.println(" end");
  
  //Serial.println(i);

  if (i >= 32 && buffer[0] == 0x42 && buffer[1] == 0x4D) {
    pm1_0 = (buffer[10] << 8) | buffer[11];
    pm2_5 = (buffer[12] << 8) | buffer[13];
    pm10  = (buffer[14] << 8) | buffer[15];
    valid_reading = true;

  } else {
    valid_reading = false;
    /*Serial.println("Erreur de lecture ou trame invalide.");
    Serial.println(i);
    Serial.println(buffer[0]);
    Serial.println(buffer[1]);*/
  }
}

void sendFile(const char* filename) {
  FsFile file = sd.open(filename);
  if (!file) {
    //Serial.println("Erreur ouverture fichier.");
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
  /*Serial.print("Serveur : ");
  Serial.println(statusCode);
  Serial.println(response);*/
}
