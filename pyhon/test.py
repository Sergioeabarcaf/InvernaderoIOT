# The following lines require manual changes
username = "idiProteinlab" # enter your username
password = "Proteinlab2017!" # enter your password
# This example establishes a https connection, but doesn't provide the server certificate validation.
# Production code should implement certificate validation.
# -------------------------------------------------

import httplib
import base64
import json
#Esta libreria se instala desde https://github.com/mikexstudios/python-firebase
from firebase import Firebase

# Se envian los datos a firebase en data realtime con la ruta del dispositivo
# y el valor de los parametros en string
def enviarFirestore(timestamp,dispositivo,data):
    dataSend = {}
    dataValues = {}
    dataSend["timestamp"] = timestamp
    for x in range (0, len(data)):
        parametro = data[x].split(":")
        dataValues[str(parametro[0])] = parametro[1]
    dataSend["values"] = dataValues
    dataSend["device"] = dispositivo
    url = "https://libelium-91af3.firebaseio.com/estacionMetereologica"
    f = Firebase(url)
    r = f.push(dataSend)

def eliminarVacios(data):
    dataSinVacios = []
    for x in range (4, len(data)):
        if(len(data[x]) > 0):
            dataSinVacios.append(data[x])
    return dataSinVacios

# Limpiar la data sacando el id del dispositivo y los parametros para enviarlos a firestore
def obtenerData(timestamp,data):
    dataLimpia = data.split("#")
    dispositivo = dataLimpia[2]
    dataLimpia = eliminarVacios(dataLimpia)
    enviarFirestore(timestamp,dispositivo,dataLimpia)


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
    timestamp = test['list'][i]['timestamp']
    obtenerData(timestamp,data)
