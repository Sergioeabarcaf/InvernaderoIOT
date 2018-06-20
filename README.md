# Proyecto invernadero IOT

Este proyecto busca sensorizar un huerto vertical y generar acciones automatizadas basadas en los datos obtenidos por sensores.
Se usa Libelium y Arduino, estas placas se comunican por medio de Xbee ZB con el Coordinador (gateway ZB-Wifi) y este envia los datos a digi remote cloud.

## V0.1

Esta versión contiene lo siguiente:
* Programas que envian temperatura, humedad y presion por medio de FRAME y TEXTO desde libelium a GW.
* Para validar el mensaje enviado se debe decodificar en base64.

## Python

### V0.2.1

En esta versión se logra lo siguiente:
* Separación de funciones para DRM y Firebase en archivos distintos
* DRM getDataDevice: permite tener la data del dispositivo
* DRM obtenerData: permite extraer los valores del dispositivo
* goToFirebase send: Envia la data a Firebase en el formato {dispositivo: {key: {timestamp, values} }}
* goToFirebase updateLast: actualiza la ultima data en el formato {parametro: {timestamp, value}}
* goToFirebase checkData: verifica que el dato a enviar no sea igual que el ultimo dato almacenado.
