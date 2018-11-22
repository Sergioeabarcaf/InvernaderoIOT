import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate('dogKey.json')
default_app = firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://invernadero-33280.firebaseio.com/'
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


#verifica el tiempo del nuevo dato y actualiza los valores
def checkData(time,id):
    urlCheck = "last/" + id
    ref = db.reference(urlCheck)
    r = ref.get()
    if (r == None):
        print("No hay datos previos")
        ref.set(time)
        return True
    else:
        if r == time:
            print "El dato es el mismo"
            return False
        else:
            if r<time:
                print"Dato nuevo"
                print"firebase:"+r+"<"+time
                ref.set(time)
                return True
            else:
                print("Dato antiguo")
                return False
