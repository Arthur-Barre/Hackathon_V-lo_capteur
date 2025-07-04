# ---
# jupyter:
#   jupytext:
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.16.4
#   kernelspec:
#     display_name: Python 3 (ipykernel)
#     language: python
#     name: python3
# ---

# %%
import numpy as np
import pandas as pd
import folium 

from geopy import distance
from folium import FeatureGroup
from folium.plugins import HeatMap
from gestion_donnees import data_pollution

# %%
# Coordonnées centrées sur Paris
latitude_center = 48.8566
longitude_center = 2.3522

# %%
#sélection selon la distance

data_drop=data_pollution.drop(columns=['Id','Time','Speed'])
n=len(data_drop)
Liste1=[]   #stocke les points intermédiaires
Mean=[]    #stocke les points moyens sur 50m
compteur=0   #la distance entre des points successifs
d_lim=50   #distance de 50m

# %%
for i in range(n-1):    
    coord1= (data_drop.loc[i,'Latitude'],data_drop.loc[i,'Longitude'])
    coord2=(data_drop.loc[i+1,'Latitude'],data_drop.loc[i+1,'Longitude'])
    d=distance.distance(coord1,coord2).m   # distance entre 2 coordonnées en mètres
    compteur += d    #ajoute la distance au nouveau point

    m=len(Liste1)
    if compteur <= d_lim :
        Liste1.append(data_drop.loc[i].values.tolist())
        
    else :
        if m==0:    #point isolé
            Mean.append(data_drop.loc[i].values.tolist())
            
        else :
            somme=[0,0,0,0,0]
            for ligne in Liste1:
                somme[0]+=ligne[0]
                somme[1]+=ligne[1]
                somme[2]+=ligne[2]
                somme[3]+=ligne[3]
                somme[4]+=ligne[4]
            Mean.append([val/m for val in somme])
        compteur=0
        Liste1=[]
        

# %%
# le dataframe sélectionné
data_50m = pd.DataFrame(Mean, columns=['Latitude', 'Longitude','PM1','PM2.5','PM10'])

# %%
data_1=data_50m.drop(columns=['PM2.5','PM10'])
data_2_5=data_50m.drop(columns=['PM1','PM10'])
data_10=data_50m.drop(columns=['PM1','PM2.5'])

# %%
#création du fond de carte
latitude = 48.8566
longitude = 2.3522
zoom=13
carte=folium.Map(location=[latitude,longitude],tiles="OpenStreetMap", zoom_start=zoom, min_zoom=12, max_zoom=17)

# %%
#création des heatmap
heat1=HeatMap(data_1, gradient={0.0: 'lightgreen', 0.3: 'green', 0.6: 'orange', 1.0: 'red'}, min_opacity=0.1, name="PM1.0", show=True)
heat2_5=HeatMap(data_2_5, gradient={0.0: 'lightblue', 0.3: 'blue', 0.6: 'yellow', 1.0: 'red'}, min_opacity=0.1, name="PM2.5", show=False)
heat10=HeatMap(data_10, gradient={0.0: 'lightgreen', 0.3: 'green', 0.6: 'orange', 1.0: 'red'}, min_opacity=0.1, name="PM10", show=False)

# %%
carte.add_child(heat1)
carte.add_child(heat2_5)
carte.add_child(heat10)
folium.LayerControl(collapsed=False).add_to(carte);

# %%
carte.save("www/carte_pollution_3.html")
