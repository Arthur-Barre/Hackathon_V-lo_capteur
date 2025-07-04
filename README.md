# Cartographie de la pollution à vélo - BikeMapping

## Description du projet
Nous avons effectué un système qui permet de s'accrocher sur un Velib et capte les taux de PM10, PM2,5 et PM1. Ces valeurs sont stockées dans une carte micro SD avec la localisation correspondant à chacune des mesures, que l'on affiche ensuite sur une carte interactive sur une page Web.

## Choix techniques :
* Capteur de pollution PMS7003 et Adafruit Ultimate GPS connectés à une carte Arduino Uno R4 Wifi
* Données écrites sur une carte micro SD, reliée elle aussi à la carte Arduino avec un lecteur Micro SD module V1.0 DFRobot
* On récupère ensuite les données qui sont traitées via folium pour être affichées sur la carte OpenStreetMap du site Internet

## Description du projet :
L'idée est la suivante : Lors d'un tour en vélo, notre système embarqué mesure le taux de particule fine à pas de temps régulier et récupère la position du point de mesure. Les données sont stockées sur une carte SD pour le temps du trajet. A la fin du tour, une fois que nous sommes connecté à notre réseau wifi, les données sont envoyés automatiquement à un serveur en ligne qui permet d'allimenter une carte intéractive qui cartographie le niveau de polution de la ville. On peut par la suite consulter cette carte actualisée par les données du tour en vélo sur notre site internet.

## Dépendances :
* pandas
* sqlite3
* numpy
* folium
* FastApi

## Utilisation :
* Capteur de pollution et GPS connectés à une carte Arduino Uno R4 Wifi
* Données écrites sur une carte micro SD, reliée elle aussi à la carte Arduino
* On récupère ensuite les données qui sont traitées pour être affichées sur la carte OpenStreetMap du site Internet

* Pour lancer le serveur, taper `uvicorn server:app`

## Description du module capteurs/Arduino
 Matériel :
 * Arduino Uno R4 Wifi
 * capteur de particules fines PMS7003
 * GPS Adafruit
 * lecteur MicroSD DFRobot

## circuit éléctronique


On branche le capteur PMS7003 sur les pins 2 et 7, et le GPS sur le hardware Serial1, soit sur les pins 0 et 1. On relie le tout au module SD. 

