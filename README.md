# Proyecto invernadero IOT

Este proyecto busca sensorizar un huerto vertical y generar acciones automatizadas basadas en los datos obtenidos por sensores. 
Se usa Libelium y Arduino, estas placas se comunican por medio de Xbee ZB con el Coordinador (gateway ZB-Wifi) y este envia los datos a digi remote cloud.

## V0.1

Esta versión contiene lo siguiente:
* Programas que envian temperatura, humedad y presion por medio de FRAME y TEXTO desde libelium a GW.
* Para validar el mensaje enviado se debe decodificar en base64.

