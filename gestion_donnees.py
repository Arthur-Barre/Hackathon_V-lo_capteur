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
# ## Schéma de la base de donnée (Pandas)
#
# La base de donnée est stocké dans une dataframe intitulé data_pollution
#
# * Id
# * Time (datetime UTC)
# * Latitude (° décimaux signé)
# * Longitude (° décimaux signé)
# * PM2.5 (µg/m³)
#
# ## Schéma de la base de donnée (SQL)
# CREATE TABLE "POLLUTION" (\
# 	"Id"	INTEGER,\
# 	"Time"	INTEGER, -- Timestamp UNIX\
# 	"Latitude"	REAL,\
# 	"Longitude"	REAL,\
# 	"PM2.5"	REAL,\
# 	PRIMARY KEY("Id" AUTOINCREMENT),\
# 	CHECK("Time")\
# );

# %%
import pandas as pd
import sqlite3

# %%
DB_SQL = sqlite3.connect("database/pollution.sqlite")

# %%
data_pollution = pd.read_sql_query("SELECT * FROM POLLUTION", DB_SQL)
data_pollution['Time'] = pd.to_datetime(data_pollution['Time'], unit='ms')

# %%
data_pollution.head()

# %%
