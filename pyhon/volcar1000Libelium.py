import goToFirebase
import drm
import json
import base64

id = ["00:13:A2:00:41:04:37:F1","00:13:A2:00:41:5B:67:EB"]
cantidad = 1000

#Recorrer el arreglo con id
for disp in id:
    # Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
    data = json.loads(drm.getDataDevice(disp,cantidad))
    # ordenar dejando el dato mas nuevo al final del array
    dataReverse = data['list'][::-1]
    for x in dataReverse:
        dataValue = base64.b64decode(x['value'])
        dataTime = x['timestamp']
        # Limpiar la data y obtener el dispositivo con sus valores
        device, data = drm.obtenerData(dataValue)
        if goToFirebase.checkData(dataTime, disp):
            goToFirebase.send(dataTime,disp,data)
