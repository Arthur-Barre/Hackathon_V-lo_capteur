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
# # Transfert des données depuis capteur

# %%
import pandas as pd
import sqlite3

# %%
"""Charge le CSV spécifié dans la base de donnée SQL

Le format attendu est 
`J,M,A,H,min,S,vitesse,latitude,longitude`
et les séparateurs décimaux des ."""
def import_csv_to_sqlDB(path_to_csv):
    df_from_csv = pd.read_csv(path_to_csv, header=None, names=["J", "M", "A", "H", "min", "s", "Vitesse", "Latitude", "Longitude"],
                              sep=',',
                              dtype={
                                  "J":str,
                                  "M":str,
                                  "A":str,
                                  "H":str,
                                  "min":str,
                                  "s":str
                              })

    return df_from_csv
    


# %%
import_csv_to_sqlDB("test/test_import.csv")[['J','M']]

# %%
SQL_DB = sqlite3.connect("database/pollution.sqlite")


# %%
def saveDFtoSQL(df):
    throw(NotImplementedError)
    #Il faut que je vérifie que les colonnes sont bonnes
    df.to_sql("POLLUTION", SQL_DB, if_exists="append", index_label="Id")

# %%
