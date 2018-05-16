// Envio de Temperatura, Humedad y Presion hacia GW via FRAME y visualizacion en DIGI REMOTE CLOUD
// La visualizacion de los datos esta en base64, decodificar para entender el mensaje
// Programa desarrollado en PROTEINLAB por Sergio Abarca F.

#include <WaspXBeeZB.h>
#include <WaspFrame.h>
#include <WaspSensorAgr_v30.h>

//MAC de GW Digi
char RX_ADDRESS[] = "0013A20040DC588F";

//ID que usara
char WASPMOTE_ID[] = "Waspmote";

//Variables a usar
uint8_t error;


void setup()
{
  //Inicializacion de objetos
  USB.ON();
  USB.println(F("Enviando Temperatura, Humedad y Presion a Coordinador FRAME"));
  xbeeZB.ON();
  Agriculture.ON();

  //Configurar ID de Frame
  frame.setID( WASPMOTE_ID );

  delay(3000);

  checkNetworkParams();
}


void loop()
{
  //Creacion de Frame en modo ASCII
  frame.createFrame(ASCII);
  frame.setFrameSize(92);

  //Agregar contenido al Frame
  frame.addSensor(SENSOR_AGR_TC, Agriculture.getTemperature());
  frame.addSensor(SENSOR_AGR_HUM, Agriculture.getHumidity());
  frame.addSensor(SENSOR_AGR_PRES, Agriculture.getPressure());


  //Mostrar Frame a enviar
  frame.showFrame();

  //Enviar Frame
  error = xbeeZB.send( RX_ADDRESS, frame.buffer, frame.length );  

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
  }
  USB.println(F("\nConectado en la red"));
  USB.println();
}

