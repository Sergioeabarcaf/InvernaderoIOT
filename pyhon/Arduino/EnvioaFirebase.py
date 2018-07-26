import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate('DogKey.json')
default_app = firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://proyecto-cote.firebaseio.com/'
})

def send(timestamp,dispositivo,data,feed_key):
    datasend = {}
    datavalues = {}
    urlDevice = 'dataDevices/' + dispositivo
    ref = db.reference(urlDevice)
    datavalues[feed_key] = data
    datasend["Timestamp"]= timestamp
    datasend["values"]= datavalues
##    
    ref.push(datasend)

def updateLast(timestamp,data,feed_key):
    param = {}
    send = {}
    ref = db.reference('last')
    send["time"] = timestamp
    send["value"] = str(data)
    param[feed_key] = send
    send = {}
    print (param)
    ref.update(param)

def checkData(time,data, feed_key):
    urlCheck = "last/" + feed_key
    ref = db.reference(urlCheck)
    r = ref.get()
    if (r == None):
        return True
    else:
        if r["time"] == time:
            return False
        else:
            return True
