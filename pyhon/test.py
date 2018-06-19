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
import goToFirebase
#Esta libreria se instala desde https://github.com/mikexstudios/python-firebase
# from firebase import Firebase
# tiempo en segundos a dormir
standby = 1800
# Almacenador de dispositivos
dispositivos = {}
# urlDispositivos = "https://libelium-91af3.firebaseio.com/dispositivos"

def eliminarVacios(data):
    dataSinVacios = []
    for x in range (4, len(data)):
        if(len(data[x]) > 0):
            dataSinVacios.append(data[x])
    return dataSinVacios

# Limpiar la data sacando el id del dispositivo y los parametros para enviarlos a firestore
def obtenerData(timestamp,data):
    dataLimpia = data.split("#")
    if ( len(dataLimpia) > 4):
        dispositivo = dataLimpia[2]
        dataLimpia = eliminarVacios(dataLimpia)
        goToFirebase.send(timestamp,dispositivo,dataLimpia)
        goToFirebase.updateLast(str(timestamp), dataLimpia)
        # if( comprobarDato(dispositivo,timestamp) ):
        #     dataLimpia = eliminarVacios(dataLimpia)
        #     enviarFirestore(timestamp,dispositivo,dataLimpia)
        # else:
        #     print "El dispositivo: " + str(dispositivo) + " con el timestamp: " + timestamp + " ya ha sido registrado."
    else:
        print "esta data no es reconocible aun"
        print data

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

    #actualizar data de dispositivos
    #
    # f = Firebase(urlDispositivos)
    # dispositivos = f.get()
    # if dispositivos == None:
    #     dispositivos = {}

    #por cada dispositivo extraer el valor y enviar a funcion obtenerData
    for i in range(1, maxI):
        data = base64.b64decode(test['list'][i]['value'])
        timestamp = test['list'][i]['timestamp']
        obtenerData(timestamp,data)
        print "se envio el " + str(i) + " de : " + str(maxI - 1)
    print "Termine! ahora me ire a dormir por " + str(standby / 60) + " minutos "
    time.sleep(standby)
