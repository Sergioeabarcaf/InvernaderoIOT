import time
import goToFirebase
import drm

# tiempo en segundos a dormir
standby = 1800

# BUG: Actualizar parametros
id =
cantidad = 1

while True:
    # Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
    data = json.loads(drm.getDataDevice(id,cantidad))
    dataValue = base64.b64decode(data['list'][0]['value'])
    dataTime = data['list'][i]['timestamp']
    # Limpiar la data y obtener el dispositivo con sus valores
    device,data = drm.obtenerData(dataTime, dataValue))
    if(device != False):
        goToFirebase.send(dataTime,device,data)
        goToFirebase.updateLast(str(dataTime), data)
    time.sleep(standby)
