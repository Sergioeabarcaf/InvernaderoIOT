import goToFirebase
import drm
import json
import base64
#import pdb

#id de arduino xbee
id = "00:13:A2:00:41:5B:67:F6"
cantidad = 1
# Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
data = json.loads(drm.getDataDevice(id,cantidad))
cantidadReal = data['count']

dataReverse=data['list'][::-1]
if(cantidad > cantidadReal):
        print "la cantidad ingresada excede los registros disponibles"
        print "Hay :",cantidadReal
for x in dataReverse:
    #pdb.set_trace()
    dataValue = base64.b64decode(x['value'])
    print dataValue
    dataTime = x['timestamp']
    print dataTime
    data = drm.obtenerDataArduino(dataTime,dataValue)
    if goToFirebase.checkData(dataTime, id):
        goToFirebase.send(dataTime,id,data)
    
        



