# Cartographie de la pollution à vélo

## Description du projet :
L'idée est la suivante : Lors d'un tour en vélo, notre système embarqué mesure le taux de particule fine à pas de temps régulier et récupère la position du point de mesure. Les données sont stockées sur une carte SD pour le temps du trajet. A la fin du tour, une fois que nous sommes connecté à notre réseau wifi, les données sont envoyés automatiquement à un serveur en ligne qui permet d'allimenter une carte intéractive qui cartographie le niveau de polution de la ville. On peut par la suite consulter cette carte actualisée par les données du tour en vélo sur notre site internet.

## Dépendances :
* pandas
* sqlite3
* numpy
* folium

## Description du module capteurs/Arduino
 Matériel :
 * Arduino Uno R4 Wifi
 * capteur de particules fines PMS7003
 * GPS Adafruit
 * lecteur MicroSD DFRobot

## circuit éléctronique


On branche le capteur PMS7003 sur les pins 2 et 7, et le GPS sur le hardware Serial1, soit sur les pins 0 et 1. On relie le tout au module SD. 

