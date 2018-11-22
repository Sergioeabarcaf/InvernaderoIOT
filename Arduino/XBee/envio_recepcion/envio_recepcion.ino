#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdlib.h>
#include <XBee.h>
#include <string.h>

#define pindht 4
#define pinhc A0
#define pintc 5
#define DHTTYPE DHT11
#define pinled 7

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

//---------Objetos

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
DHT dht (pindht, DHTTYPE);
OneWire oneWireobjeto(pintc);
DallasTemperature sensorDS18B20(&oneWireobjeto);

//--------- SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);



//variable para el estado del envio
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
//variable estado del mensaje recibido
Rx16Response rx16 = Rx16Response();

//Conversion de float a char
void floatAchar(float num, char resultado[10])
{
  //dtostrf(variable,mumero de digitos,numero de decimales,matriz);
  dtostrf(num, 5, 2, resultado);

}

//Separa los valores enviados del paquete
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

//Guarda en un arreglo cada #actuadorestado
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

long period = 1800000;
long previotiempo= 0;

void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  dht.begin();
  sensorDS18B20.begin();
  pinMode(pinled , OUTPUT);
}

void loop() {

unsigned long tiempoactual = 0;
//variable para almacenar solo la RF data del Frame
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

//tiempo
tiempoactual=millis();
if(tiempoactual - previotiempo > period)
{
  previotiempo= millis();
  // Se ajustan los valores del fc-28
  int humedad_suelo = analogRead(pinhc); 
  float porcentaje = (100*humedad_suelo)/950;
  //lectura temperatura de contacto DS18B20
  sensorDS18B20.requestTemperatures();
  //lectura de sensor dht11
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  //chequeo si son valores nulos
  if (!isnan(temperatura) && !isnan(humedad) && !isnan(humedad_suelo))
  {
    floatAchar(humedad, aux);
    strcpy(payload, "H :");
    strcat(payload, aux);
    strcat(payload, "#");
    floatAchar(temperatura, aux);
    strcat(payload, "T :");
    strcat(payload, aux);
    strcat(payload, "#");
    floatAchar(sensorDS18B20.getTempCByIndex(0), aux);
    strcat(payload, "Tc :");
    strcat(payload, aux);
    strcat(payload, "#");
    floatAchar(porcentaje, aux);
    strcat(payload, "Hc :");
    strcat(payload, aux);
    strcat(payload, "#");
    Serial.println(payload);

  }
//Envio de mensaje-----------------------------------------------------------------------

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
}
  //delay(1800000);
  //Recepcion Mensaje----------------------------------------

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
          digitalWrite(pinled, LOW);
          break;
        }
        case 4949:
         {
          Serial.println("Encender Riego");
          digitalWrite(pinled, HIGH);
          
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

  //Limpieza mensaje
  strcpy(payload, "");
  strcpy(rfdata,"");
  datosFrame ="";
  strcpy(strAscii, "");
  memset(actuadores, 0, sizeof(actuadores));
}

