#include <OneWire.h>
#include <DallasTemperature.h>

//#include <avr/dtostrf.h>
#include <stdlib.h>
#include <DHT.h>
#include <XBee.h>

#define pindht 4
#define pinhc A0
#define pintc 5
#define DHTTYPE DHT11

//---------Objetos

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
DHT dht (pindht, DHTTYPE);
OneWire oneWireobjeto(pintc);
DallasTemperature sensorDS18B20(&oneWireobjeto);

//--------- SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);

//variable mensaje
char payload[50];
//variable para el estado del envio
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

//---------------------Conversion de float a char
void floatAchar(float num, char resultado[10])
{
  //dtostrf(variable,mumero de digitos,numero de decimales,matriz);
  dtostrf(num, 5, 2, resultado);

}


char aux[10];

//30 min 1800000 ;

void setup()
{

  Serial.begin(9600);
  xbee.setSerial(Serial);
  dht.begin();
  sensorDS18B20.begin();
}

void loop()
{
  // Se ajustan los valores del fc-28
  
  int humedad_suelo = analogRead(pinhc); 
  float porcentaje = (100*humedad_suelo)/700;
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
  // ------------------------------------------ Limpieza mensaje

  strcpy(payload, "");

  delay(1800000);
}


