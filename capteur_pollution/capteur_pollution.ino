#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <SdFat.h>

//File fichier;
//String logCSV= "Temps_init,PM1.0,PM2.5,PM10\n";
bool valid_reading = false;

SoftwareSerial mySerial(D2, D7);//, false, 128);

// Objet principal pour la carte SD
SdFat sd;
FatVolume volume;
FatFile root;
// Pin CS pour la SD selon ton module (ici 4)
const int chipSelect = 4;

void setup() {
  Serial.begin(115200);
  //mySerial.begin(9600, SERIAL_8N1, 7, 8);  // sur le port 2, RX 16 / TX 17
  mySerial.begin(9600);
  
  //fichier = SD.open("/pms_data.csv", FILE_WRITE);
  //if (fichier) {
    //fichier.println("Temps_init,PM1.0,PM2.5,PM10"); // entête CSV
    //fichier.close();
  //}
  delay(1000);
  Serial.println("Initialisation...");
  
  // Nouvelle méthode d'initialisation avec SdFat v2
  if (!sd.begin(chipSelect, SD_SCK_MHZ(10))) {
    Serial.println("Échec de l'initialisation !");
    Serial.println("* Carte insérée ?");
    Serial.println("* Câblage correct ?");
    Serial.println("* ChipSelect correct ?");
    return;
  } else {
    Serial.println("Connexion SD OK !");
  }
}

const unsigned long INTERVALLE_MESURE = 2000; // 1 minute
unsigned long dernierTemps = 0;

uint8_t buffer[32];  // tableau de 32 entrées

uint16_t pm1_0 = 0, pm2_5 = 0, pm10 = 0;

void loop() {
  unsigned long tempsActuel = millis();
  if (tempsActuel - dernierTemps >= INTERVALLE_MESURE) {
    dernierTemps = tempsActuel;
    lireCapteur();
    if(valid_reading) ecrireCSV(pm1_0,pm2_5,pm10);
  }
}

void lireCapteur() {
  // Vider le buffer avant de lire sinon rempli
  while (mySerial.available()) mySerial.read();

  delay(1000);  // Laisser le capteur envoyer ses données, il émet toutes les 1s
  
  int i = 0;
  while (mySerial.available() && i < 32) {
    buffer[i++] = mySerial.read();
  }

  if (i >= 32 && buffer[0] == 0x42 && buffer[1] == 0x4D) {  
    pm1_0 = (buffer[10] << 8) | buffer[11];
    pm2_5 = (buffer[12] << 8) | buffer[13];
    pm10  = (buffer[14] << 8) | buffer[15];

    valid_reading = true;
    /*Serial.print("PM1.0: ");
    Serial.print(pm1_0);
    Serial.print(" µg/m3, PM2.5: ");
    Serial.print(pm2_5);
    Serial.print(" µg/m3, PM10: ");
    Serial.print(pm10);
    Serial.println(" µg/m3");*/
    
    //Serial.print(logCSV);
  } else {
    valid_reading = false;
    Serial.println("Erreur de lecture ou trame invalide.");
  }
}

void ecrireCSV(uint16_t  pm1_0, uint16_t  pm2_5, uint16_t pm10) {
  fichier = SD.open("data.csv", FILE_APPEND);
  if (fichier) {
    fichier.print(millis() / 1000); fichier.print(",");
    fichier.print(pm1_0); fichier.print(",");
    fichier.print(pm2_5); fichier.print(",");
    fichier.println(pm10);
    fichier.close();
  }
  //logCSV += String(millis() / 1000) + "," + String(pm1_0) + "," + String(pm2_5) + "," + String(pm10) + "\n";
}
