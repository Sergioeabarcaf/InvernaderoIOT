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
