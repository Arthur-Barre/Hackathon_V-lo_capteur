# ---
# jupyter:
#   jupytext:
#     formats: py:percent
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

# %% [markdown]
# # Gestion des données
# * Définition de la base de donnée
# * Acquisition des données
# * Aggrégation des données

# %% [markdown]
# ## Schéma de la base de donnée
#
# La base de donnée est stocké dans une dataframe intitulé data_pollution
#
# * Id
# * Time (UTC)
# * Latitude (° décimaux signé)
# * Longitude (° décimaux signé)
# * PM2.5 (µg/m³)

# %%
import pandas as pd

# %%
#Acquisition des données
data_pollution = pd.read_csv("database/test.csv", sep=";", parse_dates=["Time"], date_format="%d/%m/%Y %H:%M:%S")
#pd.to_datetime(data_pollution['Time'], format="%d/%m/%Y %H:%M:%S")
