import goToFirebase
import drm
import json
import base64

id = ["00:13:A2:00:41:5B:67:EB","00:13:A2:00:41:04:37:F1"]
cantidad = 60

#Recorrer el arreglo con id
for disp in id:
    # Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
    data = json.loads(drm.getDataDevice(disp,cantidad))
    dataReverse = data['list'][::-1]
    for x in dataReverse:
        dataValue = base64.b64decode(x['value'])
        dataTime = x['timestamp']
        # Limpiar la data y obtener el dispositivo con sus valores
        device, data = drm.obtenerData(dataValue)
        if goToFirebase.checkData(dataTime, data[0]):
            goToFirebase.send(dataTime,disp,data)
            goToFirebase.updateLast(str(dataTime), data)
        else:
            print "El dato ya habia sido cargado"
