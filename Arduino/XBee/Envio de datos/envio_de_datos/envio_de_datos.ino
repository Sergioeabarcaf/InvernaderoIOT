#include <Printers.h>
#include <XBee.h>
//Objetos

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);
uint8_t payload[]={0,0};
ZBTxRequest mensaje= ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

//conversion de float a bytes
//union u_tag {
//    uint8_t b[4];
//    float fval;
//} u;
//float type is 32bits (4 bytes) length
char FloatToBytes(float valor)
{
   char b[sizeof(valor)];
   memcpy(b, &valor, sizeof(valor));
   return b;
}


char aux;
String aux2;
void setup() {

  Serial.begin(9600);
    xbee.setSerial(Serial);
  

}

void loop() {
//lectura de sensores ejemplo
 float humedad = 5;
 float temperatura = 10;
  //chequeo si son valores nulos
 if (!isnan(temperatura) && !isnan(humedad)) 
 {
//     u.fval = humedad;
//     payload[0] = u.b; 
//   u.fval = temperatura;
//   payload[1] = u.b;
//    aux2 = String(humedad);
//    aux2.toCharArray(aux,1);
//    payload[0] = aux & 0xff;
      aux = humedad;
      payload[0] = 'temperatura'; 
      payload[1]= aux; //& 0xff;
//m;

//     aux = FloatToBytes(humedad);
//     payload[0]= aux;
 }
xbee.send(mensaje);
//luego de enviar se espera una respuesta "response"
if (xbee.readPacket(500)) 
{
// se obtuvo respuesta            
if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) 
{
  xbee.getResponse().getZBTxStatusResponse(txStatus);

  // estado del envio
  if (txStatus.getDeliveryStatus() == SUCCESS) 
  {

    Serial.println("Logrado");
  }
  else 
  {
    //El destinatario no a recibido el mensaje
    Serial.println("Mensaje perdido");
  }
}

} 
else if (xbee.getResponse().isError()) 
{
Serial.print("error al leer el paquete, codigo: ");
Serial.println(xbee.getResponse().getErrorCode());
}
else 
{
Serial.println("Esto no deberia pasar");
}

delay(5000);
}
