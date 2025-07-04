/* BikeMapping : map the pollution of your town !
 * 
 * Modules used : 
 *   esp32-s2-solo (select ESP32S2 Dev Module)
 *   pms7003 
 *   adafruit ultimate GPS breakout v3
 *   micro-SD DFR0229
 *   
 * Pins :
 *   pms7003 : 
 *     RX -> 17
 *     TX -> 16
 *   GPS :   
 *     RX -> 10
 *     TX -> 11
 *   SD card :
 *     sck -> 6
 *     MISO -> 7
 *     MOSI -> 4
 *     SS / CS -> 5
 *   
 */

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <WiFi.h>

//-------- WIFI --------//
const char *ssid = "Yum yum data";          // Change this to your WiFi SSID
const char *password = "abcdefhi";  // Change this to your WiFi password

const char *host = "80.12.85.103";        // This should not be changed
const int httpPort = 9192;                        // This should not be changed

NetworkClient client;

//-------- SD --------//
unsigned int sampling_period = 5000;// (ms)
unsigned long timer;

//-------- CAPTORS --------//
EspSoftwareSerial::UART pms7003;
EspSoftwareSerial::UART GPSUart;

TinyGPSPlus gps;

unsigned long pms_timer;

boolean valid_reading = false;
unsigned long timestamp;
float gps_lat;
float gps_lon;
float gps_speed;

uint8_t buffer[32];  // tableau de 32 entrées
int buffer_index = 0;
uint16_t pm1_0 = 0, pm2_5 = 0, pm10 = 0;

void setup() {
  rgbLedWrite(RGB_BUILTIN, 0, 0, 64);
  
  Serial.begin(115200);
  Serial.println("Hello World, let me send you some data !");

  //-------- SD card --------//
  SPI.begin(6, 7, 4, 5);
  if (!SD.begin(5)) {
    Serial.println("Card Mount Failed");
    rgbLedWrite(RGB_BUILTIN, 64, 0, 0);
    while(true);
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    rgbLedWrite(RGB_BUILTIN, 64, 0, 0);
    while(true);
  }
  Serial.println("SD Card OK");

  
  //-------- WIFI --------//
  WiFi.begin(ssid, password);

  timer = millis();
  boolean fail = false;
  while (WiFi.status() != WL_CONNECTED && !fail) {
    delay(500);
    Serial.print(".");
    if(millis() - timer > 10000) {
      Serial.println("Failed to connect to Wifi");
      fail = true;
    }
  }

  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    sendFile();
  }

  //test if data file is available
  File file = SD.open("/data.csv");
  if(!file) {
    file.close();
    file = SD.open("/data.csv", FILE_WRITE);
    file.println("#timestamp,lat,lon, speed,PM1.0,PM2.5,PM10");
  }
  file.close();

  //-------- Other modules --------//
  pms7003.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, 16, 17, false);
  Serial.println("PMS7003 OK");
  GPSUart.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, 11, 10, false);
  Serial.println("GPS OK");
  
  delay(1000);

  timer = millis();
}

void loop() {
  if(valid_reading) {
    rgbLedWrite(RGB_BUILTIN, 0, 64, 0);
  }else{
    rgbLedWrite(RGB_BUILTIN, 0, (millis() % 600) < 300 ? 64 : 0, 0);
  }
  
  readPMS7003();
  readGPS();

  if(millis() - timer > sampling_period) {
    timer += sampling_period;
    writeToCard();
  }

  delay(10);
}

void readPMS7003() {
  if(buffer_index >= 32 || millis() - pms_timer > 500) buffer_index = 0;
  
  while (pms7003.available() && buffer_index < 32) {
     buffer[buffer_index++] = pms7003.read();
     pms_timer = millis();
  }

  if (buffer_index >= 32 && buffer[0] == 0x42 && buffer[1] == 0x4D) {  
    pm1_0 = (buffer[10] << 8) | buffer[11];
    pm2_5 = (buffer[12] << 8) | buffer[13];
    pm10  = (buffer[14] << 8) | buffer[15];

    Serial.print("PM1.0: ");
    Serial.print(pm1_0);
    Serial.print(" µg/m3, PM2.5: ");
    Serial.print(pm2_5);
    Serial.print(" µg/m3, PM10: ");
    Serial.print(pm10);
    Serial.println(" µg/m3");
  } else if(buffer_index >= 32) {
    if(buffer[0] == 0x42) Serial.println("erreur 0x42");
    if(buffer[1] == 0x4D) Serial.println("erreur 0x4D");
    Serial.println("Erreur de lecture ou trame invalide.");
    valid_reading = false;
  }
}

void readGPS() {
  while (GPSUart.available() > 0) {
    if (gps.encode(GPSUart.read())) {
      valid_reading = gps.location.isValid();
      gps_lon = gps.location.lng();
      gps_lat = gps.location.lat(); 
      gps_speed = gps.speed.kmph();
      displayGPSInfo();
    }
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    rgbLedWrite(RGB_BUILTIN, 64, 0, 0);
    while(true);
  }
}

void displayGPSInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void writeToCard() {
  File file = SD.open("/data.csv", FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(!valid_reading) return;
  
  file.print(gps.time.value()); file.print(",");
  file.print(gps_lat, 6); file.print(",");
  file.print(gps_lon, 6); file.print(",");
  file.print(gps_speed, 6); file.print(",");
  file.print(pm1_0); file.print(",");
  file.print(pm2_5); file.print(",");
  file.println(pm10); 
  file.close();
}

void readResponse(NetworkClient *client) {
  unsigned long timeout = millis();
  while (client->available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client->available()) {
    String line = client->readStringUntil('\r');
    Serial.print(line);
  }

  Serial.printf("\nClosing connection\n\n");
}

void sendFile() {
  if (!client.connect(host, httpPort)) {
    return;
  }
  
  String body = "";
  File file = SD.open("/data.csv");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  int count = 0;
  while (file.available()) {
    body += char(file.read());
    count++;
  }
  file.close();

  SD.rename("/data.csv", "/old.csv");

  String postRequest = "POST " + String("/api/1.0/upload_data_file HTTP/1.1\r\n") + "Host: " + String(host) + "\r\n"
                       + "Content-Type: text/plain \r\n"
                       + "Content-Length: " + String(count) + "\r\n"
                       + "Connection: close\r\n\r\n"
                       + String(body);

  client.print(postRequest);

  for(int i = 0;i < 10;i++) {
    rgbLedWrite(RGB_BUILTIN, 0, 0, 64);
    delay(50);
    rgbLedWrite(RGB_BUILTIN, 0, 0, 0);
    delay(50);
  }
}
