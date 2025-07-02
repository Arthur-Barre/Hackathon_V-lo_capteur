#include <SPI.h>
#include <SdFat.h>

// Objet principal pour la carte SD
SdFat sd;
FatVolume volume;
FatFile root;

// Pin CS selon ton module (ici 4)
const int chipSelect = 4;

void setup()
{
  Serial.begin(9600);
  while (!Serial); // Attente pour cartes avec port série natif

  Serial.print("\nInitialisation de la carte SD...");

  pinMode(10, OUTPUT); // Requis pour les cartes type Uno

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

  // Type de carte
  uint8_t cardType = sd.card()->type();
  Serial.print("Type de carte : ");
  switch (cardType) {
    case SD_CARD_TYPE_SD1:  Serial.println("SD1"); break;
    case SD_CARD_TYPE_SD2:  Serial.println("SD2"); break;
    case SD_CARD_TYPE_SDHC: Serial.println("SDHC"); break;
    default:                Serial.println("Inconnu");
  }

  // Volume
  if (!volume.init(sd.card())) {
    Serial.println("Impossible de trouver une partition FAT16/FAT32");
    return;
  }

  // Taille du volume
  uint32_t volumesize = volume.sectorsPerCluster();
  volumesize *= volume.clusterCount();
  volumesize *= 512;

  Serial.print("Taille du volume (bytes): "); Serial.println(volumesize);
  Serial.print("Taille (Kbytes): "); Serial.println(volumesize / 1024);
  Serial.print("Taille (Mbytes): "); Serial.println(volumesize / 1024 / 1024);

  // Liste des fichiers
  Serial.println("\nFichiers trouvés sur la carte :");
  root.openRoot(&volume);
  root.ls(LS_R | LS_DATE | LS_SIZE);
}

void loop() {}
