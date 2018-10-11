#include <XBee.h> 
//---------------------------------------
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx64Response rx64 = Rx64Response();
uint8_t data;
uint8_t posicionDatos;
String auxStr;


//funcion para convertir datos uint8_t a String
String toString(uint8_t *str){
    return String((char *)str);
}


void setup() 
{
  Serial.begin(9600);
}

void loop()
{
//se espera la llegada de un paquete durante 100milliseg antes de continuar
  xbee.readPacket(100) ;
  
  if (xbee.getResponse().isAvailable()) {
    //Se leyo algo
        if (xbee.getResponse().getApiId() == RX_64_RESPONSE) {
                xbee.getResponse().getRx64Response(rx64);
                Serial.print(F("Paquete Recibido de: "));
                rx64.getRemoteAddress64();
                Serial.print(F("Señal: "));
                rx64.getRssi();
                posicionDatos = rx64.getDataOffset();
                posicionDatos = atoi(posicionDatos);
                Serial.print("Posicion-");
                Serial.print(posicionDatos);
                data=rx64.getData();
                Serial.write(rx64.getData(),rx64.getDataLength());
                Serial.println();
                Serial.println("Data-");
                Serial.println(data);    
                //particion de String
                auxStr = toString(data);
                Serial.print(auxStr);
                auxStr=auxStr.substring(posicionDatos);
                Serial.print( auxStr);

                for(int i = 0; i<xbee.getResponse().getFrameDataLength();i++)
                {
                  Serial.print(xbee.getResponse().getFrameData()[i],HEX);
                  Serial.print(" ");
                }
                
        }       
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
