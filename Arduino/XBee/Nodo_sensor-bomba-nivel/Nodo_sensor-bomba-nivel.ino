#include <XBee.h>
#include <stdlib.h>
#include <string.h>

#define relay1 6
#define relay2 5
#define relay3 4
#define relay4 3
#define sensorUltrasonido 13

//Actuadores

//1 = Riego
//2 = Ventilador1
//3 = Ventilador2
//4 = Ventilador3

//Estados
//0
//1

//Mensaje Formato

//#ActuadorEstado#Actuador2Estado2#Actuador3Estado3#Actuador4Estado4

//---------Objetos

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

//--------- SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);

//variable para el estado del envio
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
//variable estado del mensaje recibido
Rx16Response rx16 = Rx16Response();

//-------Nivel de Agua
int distancia;
unsigned long pulsoduracion = 0;
int WaterLevel = 0;
int auxWaterLevel = 0;

void Distancia()
{
   pinMode(sensorUltrasonido, OUTPUT);
   digitalWrite(sensorUltrasonido, LOW);
   delayMicroseconds(5);
  //se activa el sensor
   digitalWrite(sensorUltrasonido, HIGH);
   delayMicroseconds(5);
   digitalWrite(sensorUltrasonido, LOW);
  //se cambia de modo para imprimir
    pinMode(sensorUltrasonido, INPUT);
    pulsoduracion=pulseIn(sensorUltrasonido, HIGH);
}

//---------------------Distancia de Agua
int DistanciatoCm()
{
  Distancia();
//se lleva a cm
  pulsoduracion = pulsoduracion/2;
  distancia = int(pulsoduracion/29);
  delay(500); 
  return distancia; 
}

//---------------------Conversion de float a char
void floatAchar(float num, char resultado[10])
{
  //dtostrf(variable,mumero de digitos,numero de decimales,matriz);
  dtostrf(num, 5, 2, resultado);

}

//---------------------Separa los valores enviados del paquete
//Extrae el mensaje del frame recibido 6422088143001354948355048
String ObtenerDataFrame(String Frame, int Framelength)
{
  //Se busca en el paquete de datos el caracter # = 35(dec) --> #ActuadorEstado
  String strAux = "35";  
  String strAuxmensaje;
  int indice;
  indice = Frame.indexOf(strAux,0);
  //particiona el string en el indice encontrado de #
  strAuxmensaje = Frame.substring(indice);
  return strAuxmensaje; //354948355048
}


//---------------------Guarda en un arreglo cada #actuadorestado
void ActivarArreglo(String mensaje, int actuadores[],int inicio)
{
   //mensaje = 354948355048
   int indice;
   //Arreglo de 4 elementos
   //split segun # hasta que se retorne -1
  if(mensaje.indexOf("35")==-1)
  {

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
 }


//---------------------Envio de mensaje
void EnvioMensaje()
{
  
   if (xbee.readPacket(500))
  {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
    {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      // estado del envio
      if (txStatus.getDeliveryStatus() == SUCCESS)
      {
        Serial.println("Logrado");
      }
      else
      {//El destinatario no a recibido el mensaje
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
}  

void NiveldeAgua()
{
  char arreglo[50];
  char aux[10];
    if(auxWaterLevel- WaterLevel>=1 || WaterLevel - auxWaterLevel >=1)
    {
      WaterLevel = auxWaterLevel;
      memset(arreglo,0, sizeof(arreglo));
      strcpy(arreglo, "WaterLv :");
      floatAchar(auxWaterLevel, aux);
      strcat(arreglo, aux);
      strcat(arreglo, "#");
      ZBTxRequest mensaje = ZBTxRequest(addr64, (uint8_t*)arreglo, sizeof(arreglo));
      xbee.send(mensaje);
      EnvioMensaje();
      }
}

void setup() {
  Serial.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(sensorUltrasonido, OUTPUT);
  xbee.setSerial(Serial);
  WaterLevel = DistanciatoCm();
  Serial.print("Inicial Nivel de Agua:");
  Serial.println(WaterLevel);
}

long period = 300000;
long previotiempo= 0;

void loop() 
{
  unsigned long tiempoactual = 0;
  String datosFrame;
  String auxString;
  char strAscii[50];
  char rfdata[50];
  //Actuadores que luego se mandaran a activar
  int actuadores[] ={0,0,0,0};
  //variable mensaje
  char payload[50];
  char aux[10];
  char data[15];
  uint8_t posicionDatos;
  String auxStr;
  int auxInt;


//Recepcion Mensaje----------------------------------------

  xbee.readPacket();
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
   char arreglo[50];
   memset(arreglo, 0, sizeof(arreglo));
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
          digitalWrite(relay1, LOW);
          auxWaterLevel = DistanciatoCm();
          NiveldeAgua();
          Serial.print("Nivel de Agua:");
          Serial.println(auxWaterLevel);
          break;
        }
        case 4949:
         {
         Serial.println("Encender Riego");
          auxWaterLevel = DistanciatoCm();
          if (auxWaterLevel >=5)
          {
            strcpy(arreglo, "S1:");
            strcat(arreglo, "0");
            strcat(arreglo, "#");
            ZBTxRequest mensaje = ZBTxRequest(addr64, (uint8_t*)arreglo, sizeof(arreglo));
            xbee.send(mensaje);
            EnvioMensaje();
            delay(60000);
           }
          else
          {
            digitalWrite(relay1,HIGH);
            strcpy(arreglo, "S1:");
            strcat(arreglo, "1");
            strcat(arreglo, "#");
            ZBTxRequest mensaje = ZBTxRequest(addr64, (uint8_t*)arreglo, sizeof(arreglo));
            xbee.send(mensaje);
            EnvioMensaje();
          }
                    
          break;
         }
        case 5048:
        {
          Serial.println("Apagar Ventilador 1");
          digitalWrite(relay2, LOW);
          break;
        }
        case 5049:
         {
          Serial.println("Encender Ventilador 1");
          digitalWrite(relay2,HIGH);
          break;
         }
        case 5148:
          {
            Serial.println("apagar Ventilador 2");
            digitalWrite(relay3, LOW);
          break;
          }
        case 5149:
          {
            Serial.println("encender Ventilador 2");
            digitalWrite(relay3,HIGH);
            break;
          }
        case 5248:
        {
          Serial.println("apagar Ventilador 3");
          digitalWrite(relay4, LOW);
          
          break;
        }
        case 5249:
         {
          Serial.println("encender Ventilador 3");
          digitalWrite(relay4,HIGH);
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

  else
  {
    auxWaterLevel = DistanciatoCm();
    NiveldeAgua();
    Serial.print("aux :");
    Serial.println(WaterLevel);
    Serial.print("aux Agua:");
    Serial.println(auxWaterLevel);
    delay(5000);
    
  }
  //Limpieza mensaje
  strcpy(payload, "");
  strcpy(rfdata,"");
  datosFrame ="";
  strcpy(strAscii, "");
  memset(actuadores, 0, sizeof(actuadores));
  memset(payload,0, sizeof(payload));
  strcpy(aux, "");
}

