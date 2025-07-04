#include <SoftwareSerial.h>
#include <SPI.h>
#include <SdFat.h>

// File système via SdFat
SdFat sd;
FsFile fichier;

bool valid_reading = false;

SoftwareSerial mySerial(D2, D7);

const int chipSelect = 4;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  // Initialisation de la carte SD
  if (!sd.begin(chipSelect, SD_SCK_MHZ(10))) {
    Serial.println("Échec de l'initialisation !");
    Serial.println("* Carte insérée ?");
    Serial.println("* Câblage correct ?");
    Serial.println("* ChipSelect correct ?");
    return;
  } else {
    Serial.println("Connexion SD OK !");
  }

  // Écriture de l’en-tête du fichier CSV
  fichier = sd.open("data.csv", FILE_WRITE);
  if (fichier) {
    fichier.println("Temps_init,PM1.0,PM2.5,PM10");
    fichier.close();
  }
}



const unsigned long INTERVALLE_MESURE = 2000;
unsigned long dernierTemps = 0;

uint8_t buffer[32];
uint16_t pm1_0 = 0, pm2_5 = 0, pm10 = 0;

void loop() {
  unsigned long tempsActuel = millis();
  if (tempsActuel - dernierTemps >= INTERVALLE_MESURE) {
    dernierTemps = tempsActuel;
    lireCapteur();
    if (valid_reading) ecrireCSV(pm1_0, pm2_5, pm10);
  }
}

void lireCapteur() {
  while (mySerial.available()) mySerial.read();
  delay(1000);

  int i = 0;
  while (mySerial.available() && i < 32) {
    buffer[i++] = mySerial.read();
  }

  if (i >= 32 && buffer[0] == 0x42 && buffer[1] == 0x4D) {
    pm1_0 = (buffer[10] << 8) | buffer[11];
    pm2_5 = (buffer[12] << 8) | buffer[13];
    pm10  = (buffer[14] << 8) | buffer[15];
    valid_reading = true;
  } else {
    valid_reading = false;
    Serial.println("Erreur de lecture ou trame invalide.");
  }
}

void ecrireCSV(uint16_t pm1_0, uint16_t pm2_5, uint16_t pm10) {
  fichier = sd.open("data.csv", FILE_WRITE);
  if (fichier) {
    fichier.print(millis() / 1000); fichier.print(",");
    fichier.print(pm1_0); fichier.print(",");
    fichier.print(pm2_5); fichier.print(",");
    fichier.println(pm10);
    fichier.close();
  }
}
