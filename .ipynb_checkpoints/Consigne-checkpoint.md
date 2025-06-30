12 « Multi-capteurs: BikeMapping - Cartographie de Pollution à Vélo » MINES

Venez apprendre à utiliser des capteurs pour fabriquer un super système de cartographie de la pollution de l’air en vélo à Paris autour des mines.
Encadrant du projet

Il s’agit de réaliser un système mobile pour cartographier la pollution urbaine via des capteurs embarqués sur un vélo, avec une synchronisation centralisée des données.

Dans ce sujet plusieurs points peuvent être abordé:

    Conception d’un boîtier compact et robuste pour l’avant du vélo

    Intégration d’un capteur de pollution atmosphérique aux particules fines (PM2.5/PM10)

    Installation d’un module GPS pour une géolocalisation précise

    Stockage local des données pendant les trajets sur carte SD

    Synchronisation des données au retour via protocole LoRa ou Zigbee vers un hub central

    Hub collecteur installé dans un emplacement fixe à l’école pour centraliser les données

    Traitement par interpolation spatiale pour créer des cartes de mesures

    Dashboard local hébergé sur le hub pour visualisation sans dépendre d’internet

    Modèle d’apprentissage automatique pour prédire l’évolution des concentrations de polluants

Composants possibles

    Capteur Nova SDS011 ou alternative moins coûteuse

    Module GPS NEO-6M ou similaire

    Arduino Nano 33 BLE Sense ou ESP32 comme unité embarquée

    Module LoRa/Zigbee pour transmission à distance

    Raspberry Pi ou ODROID comme hub central de collecte

    Batterie LiPo rechargeable ou option à dynamo

    Écran optionnel pour affichage temps réel sur le vélo

Code à développer

    Firmware embarqué pour acquisition et stockage local

    Protocole de synchronisation capteur-hub

    Base de données géospatiale sur le hub

    Serveur web local pour visualisation des données

    Scripts d’analyse et apprentissage automatique