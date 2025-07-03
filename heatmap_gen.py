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

from folium import FeatureGroup
from folium.plugins import HeatMap
from gestion_donnees import data_pollution

# %%
QA=pd.read_csv("indices_QA_commune_IDF_04_2018.csv")


# %%
# Coordonnées centrées sur Paris
latitude_center = 48.8566
longitude_center = 2.3522

# %%
data_1=data_pollution.drop(columns=['Id','Time','Speed','PM2.5','PM10'])
data_2_5=data_pollution.drop(columns=['Id','Time','Speed','PM1','PM10'])
data_10=data_pollution.drop(columns=['Id','Time','Speed','PM1','PM2.5'])

# %%
latitude = 48.8566
longitude = 2.3522
zoom=13
carte=folium.Map(location=[latitude,longitude],tiles="OpenStreetMap", zoom_start=zoom, min_zoom=12, max_zoom=15)

pollution = 42

#folium.Marker(location=[latitude,longitude], popup=f"Nox ={pollution} micro_g/m3", icon=folium.Icon(icon="cloud")).add_to(carte);

# %%
def niveau_couleur(value):
    if value <= 25 :
        return 'lightgreen'
    elif 25<value<=50 :
        return 'green'
    elif 50<value<=75 :
        return 'orange'
    else :
        return 'red'

# %%
#for index, row  in test.iterrows():
    #col=niveau_couleur(row['PM2.5'])
    #folium.Marker(location=[row['Latitude'],row['Longitude']], tooltip=f"Nox = {row['PM2.5']} micro_g/m3", icon=folium.Icon(icon="cloud",color=col)).add_to(carte);
    


# %%
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
