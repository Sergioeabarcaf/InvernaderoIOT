import httplib
import base64
import json

username = "idiProteinlab"
password = "Proteinlab2017!"

def clean(data):
    dataClean = []
    for x in range (4, len(data)):
        if(len(data[x]) > 0):
            dataClean.append(data[x])
    return dataClean

# Limpiar la data sacando el id del dispositivo y los parametros para enviarlos a firestore
def obtenerData(dataTime,dataValues):
    data = data.split("#")
    if ( len(data) > 4):
        device = str(dataLimpia[2])
        data = clean(data)
        return device, data
    else:
        return False, False

#obtener datos desde DiGi remote Manager de un dispositivo especifico
def getDataDevice(id,cantidad):
    link = "/ws/v1/streams/history/00000000-00000000-00409DFF-FF6496C2/xbee.serialIn/["+ id + "]!?size=" + str(cantidad) + "&order=desc"
    auth = base64.encodestring("%s:%s"%(username,password))[:-1]
    webservice = httplib.HTTPSConnection("remotemanager.digi.com")
    webservice.putrequest("GET", link)
    webservice.putheader("Authorization", "Basic %s"%auth)
    webservice.endheaders()
    response = webservice.getresponse()
    if (response.status == 200):
        return response.read()
    else:
        return response.status
