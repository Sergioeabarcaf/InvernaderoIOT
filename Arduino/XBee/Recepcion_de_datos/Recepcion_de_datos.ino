#include <XBee.h> 
#include <string.h>
#define led 7

//--------------------------------------
//Actadores

//1 = Riego
//2 = Ventilador1
//3 = Ventilador2
//4 = Ventilador3

//Estados
//0
//1

//Mensaje Formato

//#ActuadorEstado#Actuador2Estado2#Actuador3Estado3#Actuador4Estado4

//---------------------------------------
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

char data[15];
uint8_t posicionDatos;
String auxStr;
int auxInt;

Rx16Response rx16 = Rx16Response();

//Extrae el mensaje del frame recibido 6422088143001354948355048


String ObtenerDataFrame(String Frame, int Framelength)
{
  //busca en el string el caracter # = 23 (hex)--> 35 (dec)
  //#ActuadorEstado
  String strAux = "35";  
  String strAuxmensaje;
  int indice;
  indice = Frame.indexOf(strAux,0);
  //particiona el string en el indice encontrado de #
  strAuxmensaje = Frame.substring(indice);
  return strAuxmensaje; //354948355048
}


void ActivarArreglo(String mensaje, int actuadores[],int inicio)
{
   //mensaje = 354948355048
   int indice;
   //Arreglo de 4 elementos
   do{
  if(mensaje.indexOf("35")==-1)
  {
    break;
  }
  else 
  {
    
    String strAuxmensaje = "";
    indice = mensaje.indexOf("35");
    strAuxmensaje = mensaje.substring(indice+2,indice+6);
    //4948 //5048
    mensaje = mensaje.substring(indice+4);
    //mensaje = 355048
    actuadores[inicio] = strAuxmensaje.toInt();
    ActivarArreglo(mensaje,actuadores,inicio+1);
  }
   }while(0);
 }


void setup() 
{
  pinMode(led , OUTPUT);
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


//Actuadores que luego se mandaran a activar
int actuadores[] ={0,0,0,0};



  
//se espera la llegada de un paquete durante 500milliseg antes de continuar
  xbee.readPacket(500) ;
  if (xbee.getResponse().isAvailable()) {
    //Se leyo algo
    Serial.println("LLego Mensaje");
    xbee.getResponse().getRx16Response(rx16);
    rx16.getRemoteAddress16();
    //Received Signal Strength
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
  
   ActivarArreglo(datosFrame,actuadores,0);
 
   for(int i=0;i<sizeof(actuadores) / sizeof(actuadores[0]);i++){
      
  //Ascii -- Decimal
      //48-0
      //49-1
      //50-2
      //51-3
      //52-4
      switch(actuadores[i])
      {
        case 4948:
        {
          Serial.println("Apagar Riego");
          digitalWrite(led, LOW);
          break;
        }
        case 4949:
         {
          Serial.println("Encender Riego");
          digitalWrite(led, HIGH);
          
          break;
         }
        case 5048:
        {
          Serial.println("Apagar Ventilador 1");
          break;
        }
        case 5049:
         {
          Serial.println("Encender Ventilador 1");
          break;
         }
        case 5148:
          {
            Serial.println("apagar Ventilador 2");
          break;
          }
        case 5149:
          {
            Serial.println("encender Ventilador 2");
            break;
          }
        case 5248:
        {
          Serial.println("apagar Ventilador 3");
          break;
        }
        case 5249:
         {
          Serial.println("encender Ventilador 3");
          break;
         }
        default:
          break;
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

 //Limpieza de Variables------------------------------
 strcpy(rfdata,"");
 datosFrame ="";
 strcpy(strAscii, "");
 memset(actuadores, 0, sizeof(actuadores));
}

