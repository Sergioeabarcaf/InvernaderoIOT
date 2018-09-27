import goToFirebase
import drm
import json
import base64

id = "00:13:A2:00:41:5B:67:EB"
cantidad = 1


# Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
data = json.loads(drm.getDataDevice(id,cantidad))
dataValue = base64.b64decode(data['list'][0]['value'])
dataTime = data['list'][0]['timestamp']
# Limpiar la data y obtener el dispositivo con sus valores
data = drm.obtenerDataArduino(dataTime,dataValue)

if goToFirebase.checkData(dataTime, id):
    goToFirebase.send(dataTime,id,data)
    goToFirebase.updateLast(str(dataTime), id)
else:
    print "El dato ya habia sido cargado"
