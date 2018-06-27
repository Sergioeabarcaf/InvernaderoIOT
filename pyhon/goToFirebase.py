import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate('dogKey.json')
default_app = firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://libelium-91af3.firebaseio.com/'
})

# Almacenar data en registro historico
def send(timestamp,dispositivo,data):
    dataSend = {}
    dataValues = {}
    urlDevice = 'dataDevices/' + dispositivo
    ref = db.reference(urlDevice)
    for x in range (0, len(data)):
        parametro = data[x].split(":")
        dataValues[str(parametro[0])] = parametro[1]
    dataSend["timestamp"] = timestamp
    dataSend["values"] = dataValues
    ref.push(dataSend)

# Actualizar ultimos valores obtenidos
def updateLast(timestamp,data):
    param = {}
    send = {}
    ref = db.reference('last')
    for x in range (0, len(data)):
        parametro = data[x].split(":")
        send["time"] = timestamp
        send["value"] = str(parametro[1])
        param[parametro[0]] = send
        send = {}
    print param
    ref.update(param)

def checkData(time,data):
    param = data.split(":")
    urlCheck = "last/" + param[0]
    ref = db.reference(urlCheck)
    r = ref.get()
    if (r == None):
        return True
    else:
        if r["time"] == time:
            return False
        else:
            if r["time"] < time:
                print "el dato es nuevo"
            else:
                print "el dato es viejo"
            return True
