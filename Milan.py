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

from gestion_donnees import data_pollution

# %%
QA=pd.read_csv("indices_QA_commune_IDF_04_2018.csv")


# %%
latitude = 48.8566
longitude = 2.3522
zoom=13
carte=folium.Map(location=[latitude,longitude],tiles="OpenStreetMap", zoom_start=zoom)

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
for index, row  in data_pollution.iterrows():
    col=niveau_couleur(row['PM2.5'])
    folium.Marker(location=[row['Latitude'],row['Longitude']], tooltip=f"Nox = {row['PM2.5']} micro_g/m3", icon=folium.Icon(icon="cloud",color=col)).add_to(carte);
    


# %%
carte.save("carte_pollution.html")

# %%
display(carte)

# %%
