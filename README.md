# Cartographie de la pollution à vélo - BikeMapping

## Description du projet
Nous avons effectué un système qui permet de s'accrocher sur un Velib et capte les taux de PM10, PM2,5 et PM1. Ces valeurs sont stockées dans une carte micro SD avec la localisation correspondant à chacune des mesures, que l'on affiche ensuite sur une carte interactive sur une page Web.

## Choix techniques :
* Capteur de pollution PMS7003 et Adafruit Ultimate GPS connectés à une carte Arduino Uno R4 Wifi
* Données écrites sur une carte micro SD, reliée elle aussi à la carte Arduino avec un lecteur Micro SD module V1.0 DFRobot
* On récupère ensuite les données qui sont traitées via folium pour être affichées sur la carte OpenStreetMap du site Internet

## Dépendances :
* pandas
* sqlite3
* pandas
* numpy
* folium

## Hash du commit stable
