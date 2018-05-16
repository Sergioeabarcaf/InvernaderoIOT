// Envio de Temperatura, Humedad y Presion hacia GW via TEXTO y visualizacion en DIGI REMOTE CLOUD
// La visualizacion de los datos esta en base64, decodificar para entender el mensaje
// Programa desarrollado en PROTEINLAB por Sergio Abarca F.

#include <WaspXBeeZB.h>
#include <WaspSensorAgr_v30.h>

//MAC de GW Digi
char RX_ADDRESS[] = "0013A20040DC588F";

//ID
char WASPMOTE_ID[] = "Waspmote";

//Variables a usar
uint8_t error;
float temp, hum, pres;
char tempSTR[15],humSTR[15],presSTR[15],msjSTR[100];


void setup()
{
  //Inicializacion de objetos
  USB.ON();
  xbeeZB.ON();
  Agriculture.ON();
  checkNetworkParams();
  USB.println(F("Enviando Temperatura, Humedad y Presion a Coordinador TEXTO"));
  delay(3000);
}


void loop()
{
  //Obtener temperatura, humedad, presion y transformarlo a String con 3 decimales
  temp = Agriculture.getTemperature();
  hum = Agriculture.getHumidity();
  pres = Agriculture.getPressure();
  Utils.float2String (temp, tempSTR, 3);
  Utils.float2String (hum, humSTR, 3);
  Utils.float2String (pres, humSTR, 3);
  
  //Concatenar el las variables en un solo gran mensaje que contenga identificadores de donde se envia
  // y tambien contenga un separador ($)
  strcat(msjSTR,WASPMOTE_ID);
  strcat(msjSTR,"$T:");
  strcat(msjSTR,tempSTR);
  strcat(msjSTR,"$H:");
  strcat(msjSTR,humSTR);
  strcat(msjSTR,"$P:");
  strcat(msjSTR,humSTR);
  
  //Mostrar el mensaje que se envia como flag
  USB.println(msjSTR);

  //Envio del mensaje
  error = xbeeZB.send( RX_ADDRESS, msjSTR );

  // Comprobar que el envio sea correcto
  if ( error == 0 )
  {
    USB.println(F("Envio OK"));
    USB.println(" ");

    // blink green LED
    Utils.blinkGreenLED();

  }
  else
  {
    USB.println(F("ERROR"));
    USB.println(error);
    USB.println(" ");

    // blink red LED
    Utils.blinkRedLED();
  }

  // Espera de 120 segundos para un nuevo envio de Frame
  delay(120000);
}

// Revisar parametros de conexion
void checkNetworkParams()
{
  xbeeZB.getOperating64PAN();
  xbeeZB.getAssociationIndication();
  while ( xbeeZB.associationIndication != 0 )
  {
    delay(2000);
    xbeeZB.getAssociationIndication();
  }
  USB.println(F("\nConectado en la red"));
}
