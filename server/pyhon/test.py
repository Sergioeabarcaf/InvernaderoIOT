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
def enviarFirestore(disp,data):
    algo = data.split(":")
    url = "https://libelium-91af3.firebaseio.com/" + disp
    f = Firebase(url)
    r = f.push({algo[0]: algo[1]})
    print r

# Limpiar la data sacando el id del dispositivo y los parametros para enviarlos a firestore
def obtenerData(data):
    algo = data.split("#")
    disp = algo[2]
    print algo
    for x in range (4, len(algo)):
        if(len(algo[x]) > 0):
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
