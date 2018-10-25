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
//
//int getDigitValue(char digit) {
//  int n = digit-'0';
//  if (n>=0 && n<10) { return n; }
//  if (digit=='A') { return 10; }
//  if (digit=='B') { return 11; }
//  if (digit=='C') { return 12; }
//  if (digit=='D') { return 13; }
//  if (digit=='E') { return 14; }
//  if (digit=='F') { return 15; }
//  return -1;
//}
//
//int getPairValue(char digit1, char digit2) {
//  int v1 = getDigitValue(digit1);
//  int v2 = getDigitValue(digit2);
//  return (v1==-1 || v2==-1) ? -1 : v1 * 16 + v2;
//}
//
//bool hexStrToStr(char *dst, const char *src, int size) {
//  bool isGood = false;
//  if (size % 2 == 0) {
//    isGood = true;
//    for (int i=0; i<size; i+=2) {
//      int pv = getPairValue(src[i], src[i+1]);
//      if (pv==-1) { isGood = false; break; }
//      *dst = pv;
//      dst++;
//    }
//  }
//  *dst = 0;
//  return isGood;
//}

//bool hexStrToStr(char *dst, const char *src) 
//{ 
//  return hexStrToStr(dst, src, strlen(src)); 
//}


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
  int longitud;
  
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
    
      Serial.print(rx16.getData(i),DEC); 
      auxString= auxString + String(rx16.getData(i),DEC);
    }

   
   datosFrame = ObtenerDataFrame(auxString, auxString.length());
   
   Serial.println("");
   Serial.println("RF DATA :");
   Serial.print(datosFrame);
   datosFrame.toCharArray(rfdata,50);
   Serial.print("\n");
   delay(2000);
   
 
   int j=1;
   String aux;
   String aux2;
   String aux12 = "#";
   char charAux[5];
   char charAux2;
   char w;

   //llevar a ascii como son numeros -30
   for(int i=0;i<datosFrame.length();i=i+2)
   {
      if(j<datosFrame.length())
      {
      w = datosFrame[i];
      aux = String(aux +w);
      w = datosFrame[j];
      aux2=String(aux2+w);
      aux12= aux12+aux+aux2;
      Serial.print("\n");
      Serial.print(aux12);
      delay(2000);
//      strAscii[x] = strcat(aux,rfdata[j]);
      j = j+2;
     
      }
      aux = "";
      aux2="";
      aux12=aux12+"#";
    
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

}
