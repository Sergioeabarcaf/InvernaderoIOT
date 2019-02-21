import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
import datetime

cred = credentials.Certificate('/home/InvernaderoIOT/pyhon/dogKey.json')
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

# Verificar si el dato es el ultimo obtenido y actualiza este valor
def checkData(time,id):
    urlCheck = "devices/" + id + "/last"
    ref = db.reference(urlCheck)
    r = ref.get()
    # errorSend(time,r)
    if (r == None):
        print "no hay datos anteriores"
        ref.set(time)
        return True
    else:
        if r == time:
            print "es el mismo dato"
            print "firebase: " + r + " == " + time
            return False
        else:
            if r < time:
                print "el dato es nuevo"
                print "firebase: " + r + " < " + time
                ref.set(time)
                return True
            else:
                print "el dato es viejo"
                print "firebase: " + r + " > " + time
                return False

def errorSend(act,pas):
    timeAct = datetime.datetime.strptime(act, '%Y-%m-%dT%H:%M:%S.%fZ')
    timePas = datetime.datetime.strptime(pas, '%Y-%m-%dT%H:%M:%S.%fZ')
    diff = timeAct - timePas
    print diff
