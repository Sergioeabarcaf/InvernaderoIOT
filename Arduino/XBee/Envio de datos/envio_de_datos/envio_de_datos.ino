//#include <avr/dtostrf.h>
#include <stdlib.h>
#include <XBee.h>
//Objetos

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);
//variable mensaje
char payload[50];
//estado del envio
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void floatAchar(float num, char resultado[10])
{
    
    dtostrf(num,5,3,resultado);
    
}


char aux[10];
void setup()
{

  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop()
{
  //lectura de sensores ejemplo
  float humedad = 5.15;
  float temperatura = 10.25;
  //chequeo si son valores nulos
  if (!isnan(temperatura) && !isnan(humedad))
  {
    //dtostrf(variable,mumero de digitos,numero de decimales,matriz);
    floatAchar(humedad,aux);
     strcpy(payload,"H :");
     strcat(payload,aux);
     strcat(payload,"#");
    floatAchar(temperatura,aux);
     strcat(payload,"T :");
     strcat(payload,aux);
     strcat(payload,"#");
    //Serial.println(payload);
 //   Serial.println(resultado);
  }


ZBTxRequest mensaje = ZBTxRequest(addr64, (uint8_t*)payload, sizeof(payload));
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
  strcpy(payload,"");
  
   delay(5000);
}


