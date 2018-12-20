import http.client
import base64
import json
#import firebase_admin
#from firebase_admin import db
#firebase_admin.initialize_app(options={'databaseURL': 'https://invernadero-221913.firebaseio.com/'})
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate('dogKey.json')
default_app = firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://invernadero-33280.firebaseio.com/'
})


username = "idiProteinlab"
password = "Proteinlab2017!"
id = ["00:13:A2:00:41:5B:67:F6","00:13:A2:00:41:54:B4:E8"]
cantidad = 1

#drm.py--------------------------------------------------------------
def clean(data):
    dataClean = []
    for x in range (4, len(data)):
        if(len(data[x]) > 0):
            dataClean.append(data[x])
    return dataClean

# Limpiar la data sacando el id del dispositivo y los parametros para enviarlos a firestore
def cleanArduino(data):
    dataClean = []
    for x in range((len(data) - 1)):
        dataClean.append(data[x])  
    return dataClean


def obtenerData(dataTime,dataValues):
    data = base64.b64decode(bytes(dataValues,'utf-8'))
    data = dataValues.split("#")
    if ( len(data) > 4):
        device = str(data[2])
        data = clean(data)
        return device, data
    else:
        return False, False
#obtener datos desde DiGi remote Manager de un dispositivo especifico

def obtenerDataArduino(dataTime,dataValues):
    data= dataValues.split("#")
    # H :43.00#T :12.00#
    if(len(data) > 1):
        data = cleanArduino(data)
        return data
    else:
        return False

def getDataDevice(id,cantidad):
    link = "/ws/v1/streams/history/00000000-00000000-00409DFF-FF6496C2/xbee.serialIn/["+ id + "]!?size=" + str(cantidad) + "&order=desc"
    strAux ="%s:%s"%(username,password)
    auth =(base64.b64encode(bytes(strAux,'utf-8')))
    auth = auth.decode('utf-8')
    webservice = http.client.HTTPSConnection("remotemanager.digi.com")
    webservice.putrequest("GET", link)
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.endheaders()
    response = webservice.getresponse()
    if (response.status == 200):
        return response.read()
    else:
        return response.status

#GoToFirebase.py-----------------------------------------------------
# Almacenar data en registro historico
def send(timestamp,dispositivo,data):
    dataSend = {}
    dataValues = {}
    aux = timestamp.split("-")
    urlDevice = 'dataDevices/%s/%s/%s'%(dispositivo,aux[0],aux[1]) 
    ref = db.reference(urlDevice)
    for x in range (0, len(data)):
        parametro = data[x].split(":")
        dataValues[str(parametro[0])] = parametro[1]
    if dispositivo == "00:13:A2:00:41:54:B4:E8":
        url = 'dataDevices/%s'%(dispositivo)
        ref_waterlevel= db.reference(url)
        dataSend["timestamp"] = timestamp
        dataSend["values"] = dataValues
        ref_waterlevel.set(dataSend)

    else:
        dataSend["timestamp"] = timestamp
        dataSend["values"] = dataValues
        ref.push(dataSend)


#verifica el tiempo del nuevo dato y actualiza los valores
def checkData(time,id):
    if id == "00:13:A2:00:41:54:B4:E8":
        urlCheck = "device/lastwaterlevel/"
        ref = db.reference(urlCheck)
        r = ref.get()
        if (r == None):
            print("No hay datos previos")
            ref.set(time)
            return True
        else:
            if r == time:
                print("El dato es el mismo")
                return False
            else:
                if r<time:
                    print("Dato nuevo")
                    print("firebase:"+r+"<"+time)
                    ref.set(time)
                    return True
                else:
                    print("Dato antiguo")
                    return False
        
    else:
        urlCheck = "device/last/"
        ref = db.reference(urlCheck)
        r = ref.get()
        if (r == None):
            print("No hay datos previos")
            ref.set(time)
            return True
        else:
            if r == time:
                print("El dato es el mismo")
                return False
            else:
                if r<time:
                    print("Dato nuevo")
                    print("firebase:"+r+"<"+time)
                    ref.set(time)
                    return True
                else:
                    print("Dato antiguo")
                    return False

#Arduino.py---------------------------------------

for valor in id:
    data = json.loads(getDataDevice(valor,cantidad))
    cantidadReal = data['count']

    dataReverse=data['list'][::-1]
    if(cantidad > cantidadReal):
            print("la cantidad ingresada excede los registros disponibles")
            print("Hay :",cantidadReal)
    for x in dataReverse:
        #pdb.set_trace()
        dataValue = base64.b64decode(x['value'])
        dataValue= dataValue.decode('utf-8','ignore')
        print(dataValue)
        dataTime = x['timestamp']
        print(dataTime)
        data = obtenerDataArduino(dataTime,dataValue)
        if checkData(dataTime, valor):
            send(dataTime,valor,data)

