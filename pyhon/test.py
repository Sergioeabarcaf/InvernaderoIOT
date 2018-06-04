# The following lines require manual changes
username = "idiProteinlab" # enter your username
password = "Proteinlab2017!" # enter your password
# This example establishes a https connection, but doesn't provide the server certificate validation.
# Production code should implement certificate validation.
# -------------------------------------------------

import time
import httplib
import base64
import json
#Esta libreria se instala desde https://github.com/mikexstudios/python-firebase
from firebase import Firebase
# tiempo en segundos a dormir
standby = 18
# Almacenador de dispositivos
dispositivos = {}

# Validadores

# Validador de que el dato de DRM no ha cambiado.
def comprobarDato(dispositivo, timestamp):
    if len(dispositivos) == 0:
        dispositivos[dispositivo] = timestamp
        print "no existen dispositivos almacenados"
        return True
    keys = dispositivos.keys()
    for disp in keys:
        if(dispositivo == disp):
            if(timestamp == dispositivos[disp]):
                print "el dato ya esta almacenado"
                return False
            else:
                print "el dispositivo esta pero este dato es nuevo"
                dispositivo[disp] = timestamp
                return True
    print "el dispositivo no esta registrado, pero se almacena"
    dispositivos[dispositivo] = timestamp
    return True

# Se envian los datos a firebase en data realtime con la ruta del dispositivo
# y el valor de los parametros en string
def enviarFirestore(timestamp,dispositivo,data):
    dataSend = {}
    dataValues = {}
    for x in range (0, len(data)):
        parametro = data[x].split(":")
        dataValues[str(parametro[0])] = parametro[1]
    dataSend["timestamp"] = timestamp
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
    if( comprobarDato(dispositivo,timestamp) ):
        dataLimpia = eliminarVacios(dataLimpia)
        enviarFirestore(timestamp,dispositivo,dataLimpia)
    else:
        print "El dispositivo: " + str(dispositivo) + " con el timestamp: " + timestamp + " ya ha sido registrado."

while True:
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
        print "se envio el " + str(i) + " de : " + str(maxI - 1)
    print "Termine! ahora me ire a dormir por " + str(standby / 60) + " minutos "
    time.sleep(standby)
