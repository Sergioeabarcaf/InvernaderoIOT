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

### libelium

### V0.2.2
En esta versión se logra lo siguiente:
* Creación de diversos programas para waspmote donde se envian datos por Frame, Texto en ZB, obtencion de Temperatura, Humedad, Presion y CO2
* Creación de programa para enviar CO2 de waspmote con Sensor Board via ZB (ZB_Frame_CO2)
* Creación de programa para enviar Temperatura, Humedad y Presión de waspmote con Agriculture Board via ZB (ZB_Frame_THP)
* La data enviada es cada 30 minutos. 
