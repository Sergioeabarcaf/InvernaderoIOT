import goToFirebase
import drm

id = "00:13:A2:00:41:5B:67:EB"
cantidad = 1

# Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
data = json.loads(drm.getDataDevice(id,cantidad))
dataValue = base64.b64decode(data['list'][0]['value'])
dataTime = data['list'][i]['timestamp']
# Limpiar la data y obtener el dispositivo con sus valores
device,data = drm.obtenerData(dataTime, dataValue))
if(device != False):
    goToFirebase.send(dataTime,device,data)
    goToFirebase.updateLast(str(dataTime), data)
else:
    print "esta data no es reconocible aun"
    print dataError
