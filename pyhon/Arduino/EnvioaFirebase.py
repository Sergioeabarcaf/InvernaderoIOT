import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate('DogKey.json')
default_app = firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://proyecto-cote.firebaseio.com/'
})

# envio de datos recibe tres listas valores, parametros (feed_key),tiempo de creacion
def send(time,dispositivo,valores,parametros):
    datasend = {}
    datavalues = {}
    urlDevice = 'dataDevices/' + dispositivo
    ref = db.reference(urlDevice)
    for i in range(len(parametros)):
        datavalues[parametros[i]] = valores[i]
        datasend["Timestamp"]= time[i]
    datasend["values"]= datavalues
##
    print(datasend)
    ref.push(datasend)

def updateLast(time,valores,parametros):
    param = {}
    send = {}
    ref = db.reference('last')
    for i in range(len(parametros)):
        send["time"] = time[i]
        send["value"] = str(valores[i])
        param[parametros[i]] = send
        send = {}
        print (param)
    ref.update(param)

def checkData(time,valores,parametros):
    for i in range(len(parametros)):
        urlCheck = "last/" + parametros[i]
        ref = db.reference(urlCheck)
        r = ref.get()
        if (r == None):
            return True
        else:
            if r["time"] == time[i]:
                return False
            else:
                return True
