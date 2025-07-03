# Cartographie de la pollution à vélo

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
