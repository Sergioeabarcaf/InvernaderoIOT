#include <XBee.h> 
#include <string.h>

//---------------------------------------
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

char data[15];
uint8_t posicionDatos;
String auxStr;
int auxInt;
Rx16Response rx16 = Rx16Response();

//Extrae el mensaje del frame recibido
String ObtenerDataFrame(String Frame, int Framelength)
{
  //busca en el string el caracter # = 23 (hex)--> 35 (dec)
  String strAux = "35";  
  String strAuxmensaje;
  int indice;
  indice = Frame.indexOf(strAux,0);
  strAuxmensaje = Frame.substring(indice +2);
  return strAuxmensaje;
}


void ActivarArreglo(String mensaje, int actuadores[])
{
   int i = 0;
   int indice;
   //Arreglo de 4 elementos
  while(mensaje.indexOf("#")!=-1)
  {
    String strAux = "#";  
    String strAuxmensaje = "";
    indice = mensaje.indexOf(strAux);
    strAuxmensaje = mensaje.substring(indice+1);
    mensaje = mensaje.substring(indice+2);
    Serial.print("\n");
    Serial.print(mensaje.indexOf("#"));
    actuadores[i] = strAuxmensaje.toInt()- 48;
    Serial.print("'\n'Actuadores:");
    Serial.print(actuadores[i]);
    i= i+1;
    Serial.print("\n");
    Serial.print("While.....");
  }
  Serial.print("'\n'Fuera del While");
}


void setup() 
{
  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop()
{
 
  //variable para almacenar solo la RF data del Frame
String datosFrame;
String auxString;
char strAscii[50];
char rfdata[50];
int j=1;
String aux;
String aux2;
String aux12 = "#";
char charAux[5];
char charAux2;
char w;
int actuadores[] ={0,0,0,0};

  
//se espera la llegada de un paquete durante 500milliseg antes de continuar
  xbee.readPacket() ;
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
    
      Serial.print(rx16.getData(i),DEC); 
      auxString= auxString + String(rx16.getData(i),DEC);
    }

   
   datosFrame = ObtenerDataFrame(auxString, auxString.length());
   
   Serial.println("");
   Serial.println("RF DATA :");
   Serial.print(datosFrame);
   datosFrame.toCharArray(rfdata,50);
   Serial.print("\n");
   delay(200);
  
   //llevar a ascii como son numeros -48
   for(int i=0;i<datosFrame.length();i=i+2)
   {
      if(j<datosFrame.length())
      {
        //w char auxiliar para concatenar
      w = datosFrame[i];
      aux = String(aux +w);
      w = datosFrame[j];
      aux2=String(aux2+ w);
      aux12= aux12+aux+aux2;
      Serial.print("\n");
      Serial.print(aux12);
      delay(2000);
      j = j+2;
      }
      aux = "";
      aux2="";
      aux12=aux12+"#";
    
   }
  
   ActivarArreglo(aux12,actuadores);
   Serial.print("\n");
   Serial.println("Arreglo");
   for(int i=0;i<sizeof(actuadores) / sizeof(actuadores[0]);i++){
      Serial.print("\n");
      Serial.print(actuadores[i]);
   }

   
   
  // Serial.println(hexStrToStr(strAscii,rfdata));
   //Serial.println(strAscii);
   
  }
  else if(xbee.getResponse().isError()){
   /*-------------------
    1 - CHECKSUM_FAILURE
    2 - PACKET_EXCEEDS_BYTE_ARRAY_LENGTH
    3 - UNEXPECTED_START_BYTE*/
    Serial.print("Codigo de Error: ");
    Serial.print(xbee.getResponse().getErrorCode(),DEC);
  }

 strcpy(rfdata,"");
 datosFrame ="";
 strcpy(strAscii, "");
 memset(actuadores, 0, sizeof(actuadores));
}
