import EnvioaFirebase
import ExtraerDatosAdafruit
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

device = "sim808"
valores = []
Parametros = []
time = []
Parametros,valores,time = ExtraerDatosAdafruit.obtenerData(Parametros,valores,time)
# Limpiar la data y obtener el dispositivo con sus valores



if EnvioaFirebase.checkData(time,valores,Parametros):
    EnvioaFirebase.send(time,device,valores,Parametros)
    EnvioaFirebase.updateLast(time, valores, Parametros)
else:
    print ("El dato ya habia sido cargado")

