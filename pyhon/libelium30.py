import goToFirebase
import drm
import json
import base64
import time

id = ["00:13:A2:00:41:04:37:F1","00:13:A2:00:41:5B:67:EB"]
cantidad = 1

while (True):
    for disp in id:
        # Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
        data = json.loads(drm.getDataDevice(disp,cantidad))
        if data['list']:
            dataValue = base64.b64decode(data['list'][0]['value'])
            dataTime = data['list'][0]['timestamp']
            # Limpiar la data y obtener el dispositivo con sus valores
            device,data = drm.obtenerData(dataValue)
            if goToFirebase.checkData(dataTime, disp):
                goToFirebase.send(dataTime,disp,data)
    time.sleep(60)
