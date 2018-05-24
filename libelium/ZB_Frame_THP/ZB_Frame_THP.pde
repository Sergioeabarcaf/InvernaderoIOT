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

  //Configurar ID de Frame
  frame.setID( WASPMOTE_ID );

  xbeeZB.ON();
  checkNetworkParams();
  
  delay(1000);
}


void loop()
{

  //Creacion de Frame en modo ASCII
  frame.createFrame(ASCII);
  frame.setFrameSize(92);

  // Obtener Temp, Hum y Press para agregar a Frame
  Agriculture.ON();
  delay(500);

  frame.addSensor(SENSOR_AGR_TC, Agriculture.getTemperature());
  frame.addSensor(SENSOR_AGR_HUM, Agriculture.getHumidity());
  frame.addSensor(SENSOR_AGR_PRES, Agriculture.getPressure());
  
  Agriculture.OFF();
  delay(500);
  
  // visualizar Frame
  frame.showFrame();

  //Enviar Frame via ZB
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

  // Dormir por 3 minutos
  USB.println("Me voy a dormir una pequeña siesta :). Ya vengo! ");
  PWR.deepSleep("00:00:03:00", RTC_OFFSET, RTC_ALM1_MODE1, ALL_OFF);
  USB.ON();
  USB.println("Ya desperte!!!!");
  
}

// Revisar parametros de conexion
void checkNetworkParams()
{
  // 1. get operating 64-b PAN ID
  xbeeZB.getOperating64PAN();

  // 2. wait for association indication
  xbeeZB.getAssociationIndication();
 
  while( xbeeZB.associationIndication != 0 )
  { 
    delay(2000);
    
    // get operating 64-b PAN ID
    xbeeZB.getOperating64PAN();   
    xbeeZB.getAssociationIndication();
  }

  USB.println("Conecto a la red!");

}
