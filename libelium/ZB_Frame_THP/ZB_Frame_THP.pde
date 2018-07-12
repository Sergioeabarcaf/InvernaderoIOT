/*
 * Obtener Temperatura, Humedad y Presión
 * en un Frame via ZB
 * Programa desarrollado en PROTEINLAB por Sergio Abarca F 
 */

//====================================================================
//      Librerias a utilizar

#include <WaspSensorAgr_v30.h>
#include <WaspXBeeZB.h>
#include <WaspFrame.h>

//====================================================================
//      Configuracion ZB y Frame

//MAC de GW Digi
char RX_ADDRESS[] = "0013A20040DC588F";

//ID que usara
char WASPMOTE_ID[] = "WaspTHP";

//Variables a usar
uint8_t error;

//====================================================================

void setup()
{
  //Inicializacion de objetos
  USB.ON();
  USB.println("Obtener Temperatura, Humedad y Presión");
  USB.println("Enviar Frame con datos obtenidos via ZB.");

  Agriculture.ON();

  //Inicializar Xbee
  xbeeZB.ON();
  checkNetworkParams();

  //Configurar ID de Frame
  frame.setID( WASPMOTE_ID );
}


void loop()
{
  //Creacion de Frame en modo ASCII
  frame.createFrame(ASCII);
  frame.setFrameSize(92);
  //Cargar los parametros al frame
  frame.addSensor(SENSOR_AGR_TC, Agriculture.getTemperature());
  frame.addSensor(SENSOR_AGR_HUM, Agriculture.getHumidity());
  frame.addSensor(SENSOR_AGR_PRES, Agriculture.getPressure());
  //Mostrar Frame
  frame.showFrame();
  
   //Enviar por ZB
  error = xbeeZB.send( RX_ADDRESS, frame.buffer, frame.length );
  // Comprobar que el envio sea correcto
  if ( error == 0 )
  {
    USB.println("Envio OK");
  }
  else
  {
    USB.println("ERROR");
    USB.println(error);
  }

  //Dormir durante un tiempo
  PWR.deepSleep("00:00:30:00",RTC_OFFSET, RTC_ALM1_MODE4, ALL_ON);
}

// Revisar parametros de conexion
void checkNetworkParams()
{
  xbeeZB.getOperating64PAN();
  xbeeZB.getAssociationIndication();
  while( xbeeZB.associationIndication != 0 )
  { 
    delay(2000);
    xbeeZB.getOperating64PAN();   
    xbeeZB.getAssociationIndication();
  }
  USB.println("Conecto a la red!");
}
