import EnvioaFirebase
import ExtraerDatosAdafruit
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

feed_key = "dht22-hum"
device = "sim808"

valor,dataTime = ExtraerDatosAdafruit.obtenerData(feed_key)
# Limpiar la data y obtener el dispositivo con sus valores



if EnvioaFirebase.checkData(dataTime, valor,feed_key):
    EnvioaFirebase.send(dataTime,device,valor,feed_key)
    EnvioaFirebase.updateLast(str(dataTime), valor, feed_key)
else:
    print ("El dato ya habia sido cargado")

