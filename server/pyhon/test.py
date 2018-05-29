# The following lines require manual changes
username = "idiProteinlab" # enter your username
password = "Proteinlab2017!" # enter your password
# This example establishes a https connection, but doesn't provide the server certificate validation.
# Production code should implement certificate validation.
# -------------------------------------------------
import httplib
import base64
import json

import firebase_admin
from firebase_admin import credentials
from firebase_admin import db


cred = credentials.Certificate('libelium-91af3-firebase-adminsdk-uw61q-d655a2f86e.json')
default_app = firebase_admin.initialize_app(cred)


# Se envian los datos a firebase en data realtime con la ruta del dispositivo
# y el valor de los parametros en string
def enviarFirestore(disp,data):
    algo = data.split(":")
    root = db.reference()
    new_user = root.child(disp).push({

    })

# Limpiar la data sacando el id del dispositivo y los parametros para enviarlos a firestore
def obtenerData(data):
    algo = data.split("#")
    disp = algo[2]
    for x in range (4, len(algo)):
        enviarFirestore(disp, algo[x])


#obtener datos desde DiGi remote Manager
auth = base64.encodestring("%s:%s"%(username,password))[:-1]
webservice = httplib.HTTPSConnection("remotemanager.digi.com")
webservice.putrequest("GET", "/ws/v1/streams/inventory")
webservice.putheader("Authorization", "Basic %s"%auth)
webservice.endheaders()
response = webservice.getresponse()
response_body = response.read()

#transformar el texto a JSON
test = json.loads(response_body)

#Contar la cantidad de dispositivos que existen en el registro
maxI = int(test['count'])

#por cada dispositivo extraer el valor y enviar a funcion obtenerData
for i in range(1, maxI):
    data = base64.b64decode(test['list'][i]['value'])
    obtenerData(data)
