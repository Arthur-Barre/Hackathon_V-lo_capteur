# Cartographie de la pollution à vélo

## Dépendances :
* pandas
* sqlite3
* numpy
* folium

# Description du module capteurs/Arduino
 Matériel :
 * Arduino Uno R4 Wifi
 * capteur de particules fines PMS7003
 * GPS Adafruit
 * lecteur MicroSD DFRobot

L'idée est la suivante : lors d'un tour en vélo, 
On branche le capteur PMS7003 sur les pins 2 et 7, et le GPS sur le hardware Serial1, soit sur les pins 0 et 1. On relie le tout au module SD. 

