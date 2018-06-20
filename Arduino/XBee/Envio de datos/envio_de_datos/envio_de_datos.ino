//#include <avr/dtostrf.h>
#include <stdlib.h>
#include <XBee.h>
//Objetos

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);
char* payload= "";
ZBTxRequest mensaje = ZBTxRequest(addr64, (uint8_t*)payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

union {
  float f;
  byte b[4];
} valor;

union FloatChar {
  float f;
  char c[sizeof(float)];
};
//char FloatToBytes(float valor)
//{
//   char b[sizeof(valor)];
//   memcpy(b, &valor, sizeof(valor));
//   return b;
//}

char* aux;
char aux2[10];
String mssg = "V ";
char resultado[50];
void setup()
{

  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop()
{
  //lectura de sensores ejemplo
  float humedad = 5.0;
  float temperatura = 10;
  //chequeo si son valores nulos
  if (!isnan(temperatura) && !isnan(humedad))
  {
//    aux2 = "H ";
    dtostrf(humedad, 4, 2, resultado);
////    aux = resultado;
//    FloatChar fc;
//    fc.f = humedad;
//    strcpy(resultado, fc.c);
//    humedad += 1.01;
//    strcat(payload, aux2);
//    strcat(payload, resultado);
      //mssg= mssg + "H : " + humedad;
      //mssg.toCharArray(resultado,50);
      Serial.println(resultado);
      strcpy(payload, resultado);

  }

  xbee.send(mensaje);
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

  strcpy(payload, "");
  strcpy(resultado, "");
  mssg = "V";
  delay(5000);
}
