#include <SD.h>
#include <SPI.h>

File fichier;

void setup() {
  Serial.begin(9600)
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // sur le port 2, RX 16 / TX 17

  
  fichier = SD.open("/pms_data.csv", FILE_WRITE);
  if (fichier) {
    fichier.println("Temps,PM1.0,PM2.5,PM10"); // entête CSV
    fichier.close();

  delay(1000)
  Serial.println("Initialisation...")
  }
}

const unsigned long INTERVALLE_MESURE = 60000; // 1 minute
unsigned long dernierTemps = 0;

uint8_t buffer[32];  // tableau de 32 entrées

uint16_t pm1_0 = 0, pm2_5 = 0, pm10 = 0;

void loop() {
  unsigned long tempsActuel = millis();
  if (tempsActuel - dernierTemps >= INTERVALLE_MESURE) {
    dernierTemps = tempsActuel;
    lireCapteur();
    ecrireCSV(pm1.0,pm2_5,pm10);
  }
}

void lireCapteur() {
  // Vider le buffer avant de lire
  while (Serial2.available()) Serial2.read();

  delay(1000);  // Laisser le capteur envoyer ses données

  int i = 0;
  while (Serial2.available() && i < 32) {
    buffer[i++] = Serial2.read();
  }

  if (i >= 32 && buffer[0] == 0x42 && buffer[1] == 0x4D) {  
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
  } else {
    Serial.println("Erreur de lecture ou trame invalide.");
  }
}

void ecrireCSV(uint16_t  pm1_0, uint16_t  pm2_5, uint16_t pm10) {
  fichier = SD.open("/pms_data.csv", FILE_APPEND);
  if (fichier) {
    fichier.print(millis() / 1000); fichier.print(",");
    fichier.print(pm1_0); fichier.print(",");
    fichier.print(pm2_5); fichier.print(",");
    fichier.println(pm10);
    fichier.close();
  }
}
