from fastapi import FastAPI, Request
from datetime import datetime, timezone
from import_data import import_csv_to_sqlDB
from fastapi.staticfiles import StaticFiles
import os

# pour faire tourner ce serveur, vous pouvez exécuter dans un shell system (CMD, PowerShell, GitBash, Bash, Zsh, ...)
# uvicorn server:app

app = FastAPI()

"""Dossier dans lequel les csv sont importés"""
FOLDER_FOR_IMPORT = "database/imported_files/"

##########################
### Serveur Web      #####
##########################
app.mount("/", StaticFiles(directory="www", html=True), name="index.html")

"""Récupère le contenu du body de la requête POST et l'enregistre un CSV.
Cf. import_data.py pour le format attendu.

Si l'entête 'File-name' est fourni, ce sera le nom du fichier. Sinon il sera généré en fonction de l'heure."""
@app.post("/api/1.0/upload_data_file", status_code=201)
async def save_the_data(request: Request):
	body = await request.body()
	#Si le nom est fournit, on le prend, sinon on en fixe un au temps
	try:
		File_name = request.headers['File-name']
	except:
		File_name = datetime.now(tz=timezone.utc).isoformat(sep=' ', timespec='milliseconds')[:-6] +'.csv'

	try:
		file = open(FOLDER_FOR_IMPORT + File_name, "wb")
		file.write(body)
		file.close()
	except:
		print(f"Erreur de sauvegarde du fichier {File_name}. Pas d'import essayé.")
		os.rename(FOLDER_FOR_IMPORT+File_name, FOLDER_FOR_IMPORT+File_name+".err")
		return "Error. File not uploaded"
	
	try:
		import_csv_to_sqlDB(FOLDER_FOR_IMPORT + File_name)
	except BaseException as err:
		print(f"Erreur de chargement dans la base de donnée du fichier {File_name}.")
		os.rename(FOLDER_FOR_IMPORT+File_name, FOLDER_FOR_IMPORT+File_name+".warn")
		print(err)
		return "Warning. The file has been uploaded but could not be included in the database."
	return "Success."
