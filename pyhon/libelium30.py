import goToFirebase
import drm
import json
import base64
import time
import conection
import timeCustom
import log
import sys

id = ["00:13:A2:00:41:04:37:F1","00:13:A2:00:41:5B:67:EB"]

try:
    while (True):
        if (conection.valid()):
            for disp in id:
                cant = goToFirebase.validDiference30Minutes(disp) * 2
                # Obtener el ultimo dato desde Digi Remote y convertirla en un JSON
                data = json.loads(drm.getDataDevice(disp,cant))
                dataReverse = data['list'][::-1]
                for x in dataReverse:
                    dataValue = base64.b64decode(x['value'])
                    dataTime = x['timestamp']
                    # Limpiar la data y obtener el dispositivo con sus valores
                    device, data = drm.obtenerData(dataValue)
                    if goToFirebase.checkData(dataTime, disp):
                        goToFirebase.send(dataTime,disp,data)
        else:
            log.recivedLog(timeCustom.getCurrenDateAndTimeSTR())
        time.sleep(60)
except:
    log.recivedExcept(timeCustom.getCurrenDateAndTimeSTR(), sys.exc_info())