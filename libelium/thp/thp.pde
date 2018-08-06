/*
 * Obtener Temperatura, Humedad y Presión
 * en un Frame via ZB
 * Programa desarrollado en PROTEINLAB por Sergio Abarca F 
*/

//      Librerias a utilizar
#include <WaspFrame.h>
#include <WaspXBeeZB.h>
#include <WaspSensorAgr_v30.h>
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
  //Inicializador de objetos
  USB.ON();
  Agriculture.ON();
  xbeeZB.ON();
  //Verificar la conexion a la red
  checkNetworkParams();
  //Configurar Frame
  frame.setID( WASPMOTE_ID );
}


void loop()
{
  frame.createFrame(ASCII);
  frame.setFrameSize(92);
  //Cargar parametros al frame
  frame.addSensor(SENSOR_AGR_TC, Agriculture.getTemperature());
  frame.addSensor(SENSOR_AGR_HUM, Agriculture.getHumidity());
  frame.addSensor(SENSOR_AGR_PRES, Agriculture.getPressure());
  frame.addTimestamp();
  frame.showFrame();

  //Enviar
  error = xbeeZB.send(RX_ADDRESS, frame.buffer, frame.length);
  if(error == 0){
    USB.println("Envio OK");
  }
  else{
    USB.print("ERROR: ");
    USB.println(error);
  }

  //Dormir durante un tiempo
  PWR.deepSleep("00:00:30:00",RTC_OFFSET, RTC_ALM1_MODE4, ALL_ON);

  // Verificar flag luego de despertar
  if( intFlag & RTC_INT )
  {
    USB.println("Borrando Flag");
    intFlag &= ~(RTC_INT);
  }
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
