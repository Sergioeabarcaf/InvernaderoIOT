#include <XBee.h> 
//---------------------------------------
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

char data[15];
uint8_t posicionDatos;
String auxStr;
int auxInt;
Rx16Response rx16 = Rx16Response();

//funcion para convertir datos uint8_t a String
String toString(uint8_t *str){
    return String((char *)str);
}


void setup() 
{
  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop()
{
//se espera la llegada de un paquete durante 100milliseg antes de continuar
  xbee.readPacket(500) ;
  if (xbee.getResponse().isAvailable()) {
    //Se leyo algo
    Serial.println("LLego Mensaje");
    xbee.getResponse().getRx16Response(rx16);
    rx16.getRemoteAddress16();
    Serial.println(F("Señal: "));
    rx16.getRssi();
    Serial.print("Mensaje:");
    for (int i = 0; i < rx16.getDataLength(); i++) 
    { 
      Serial.print(rx16.getData(i),HEX); 
      auxStr= auxStr + String(rx16.getData(i),HEX);
    }
    Serial.println(""); 
    Serial.print("Mensaje:");
    Serial.println(auxStr);
    posicionDatos = rx16.getDataOffset();
    Serial.println("Posicion Datos:");
    auxInt=atoi(posicionDatos);
    Serial.print(auxInt);
    
  }
    
  else if(xbee.getResponse().isError()){
   /*-------------------
    1 - CHECKSUM_FAILURE
    2 - PACKET_EXCEEDS_BYTE_ARRAY_LENGTH
    3 - UNEXPECTED_START_BYTE*/
    Serial.print("Codigo de Error: ");
    Serial.print(xbee.getResponse().getErrorCode(),DEC);
  }
               

}
