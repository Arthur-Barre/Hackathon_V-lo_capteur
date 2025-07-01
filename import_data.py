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
import datetime
SQL_DB = sqlite3.connect("database/pollution.sqlite")

# %%
"""Charge le CSV spécifié dans la base de donnée SQL

Le format attendu est 
`Time,vitesse,latitude,longitude,PM1,PM2.5,PM10`
et les séparateurs décimaux des .
Time est un timestamp unix UTC. (en millisecondes)

Pas d'en-têtes dans le fichier, mais vous pouvez mettre des commentaires avec '#'
"""
def import_csv_to_sqlDB(path_to_csv):

    df_from_csv = pd.read_csv(path_to_csv, header=None, names=["Time", "Speed", "Latitude", "Longitude", "PM1", "PM2.5", "PM10"],
                            sep=',',
                            dtype={
                                'Time':int,
                                'Speed':float,
                                'Latitude':float,
                                'Longitude':float,
                                'PM1':float,
                                'PM2.5':float,
                                'PM10':float
                            },
                            comment='#')
    
    if not df_from_csv.notna().values.all():
        raise(ValueError("NaN value trouvées dans le document, import annulé"))

    #En fait on parse pas les dates parce qu'on reçoit déjà un timestamp UTC
    #df_from_csv['Time'] = pd.to_datetime(df_from_csv['Time'], unit='ms', utc=True)
    
    df_from_csv.to_sql("POLLUTION", SQL_DB, if_exists="append", index=False, dtype={'Time':'INTEGER'})

    


# %%
if __name__ == '__main__':
    #C'est-à-dire si on est lancé directement dans le terminal et pas importé"
    import sys
    if len(sys.argv) == 1:
        print("Vous pouvez donner un fichier csv à importer dans la base de donnée\npython import_data.py nom-fichier.csv")
        exit(0)
    if len(sys.argv) > 2:
        print("Seulement un seul argument supporté")
        exit(1)
    import_csv_to_sqlDB(sys.argv[1])


# %%

# %%
