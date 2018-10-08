/*
 * Programa desarrollado por Sergio Abarca F
 * 
  Obtencion de CO2 siguiendo la documentacion de Gases PRO v30 Board Guide
  http://www.libelium.com/downloads/documentation/gases_sensor_board_pro.pdf
  Uso de Frame
  http://www.libelium.com/downloads/documentation/data_frame_guide.pdf
  Uso de acelerometro
  http://www.libelium.com/downloads/documentation/waspmote-accelerometer-programming_guide.pdf
  
*/

//      Librerias a utilizar
#include <WaspSensorGas_Pro.h>
#include <WaspXBeeZB.h>
#include <WaspFrame.h>
//====================================================================

//      Configuracion ZB y Frame

//MAC de GW Digi
char RX_ADDRESS[] = "0013A20040DC588F";
//ID que usara
char WASPMOTE_ID[] = "W2THP";
//Variables a usar
uint8_t error;
//====================================================================

//      Configuracion ZB y Frame

//Inicializador Sensor Board con CO2
Gas CO2(SOCKET_1);
//====================================================================

void setup() { }

void loop()
{
  Utils.setLED(LED1, LED_ON);
  PWR.deepSleep("00:00:01:00",RTC_OFFSET, RTC_ALM1_MODE4, ALL_ON);

  setupON();
  
  USB.println("Encendio bien, esperar 120 segundos para calentar sensor CO2");
  delay(120000);

  //Cargar los parametros al frame
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  //frame.addSensor(SENSOR_ACC, ACC.getX(),ACC.getY(),ACC.getZ());
  frame.addSensor(SENSOR_GASES_PRO_TC, CO2.getTemp(1));
  frame.addSensor(SENSOR_GASES_PRO_HUM, CO2.getHumidity());
  frame.addSensor(SENSOR_GASES_PRO_PRES, CO2.getPressure());
  frame.addSensor(SENSOR_GASES_PRO_CO2, CO2.getConc());
  
  //Mostrar Frame
  frame.showFrame();

  //Enviar por ZB
  error = xbeeZB.send( RX_ADDRESS, frame.buffer, frame.length );
  // Comprobar que el envio sea correcto
  if ( error == 0 )
  {
    USB.println("Envio OK");
    Utils.blinkGreenLED(330, 3);
  }
  else
  {
    Utils.blinkLEDs(30000);
    USB.println("ERROR");
    USB.println(error);
  }

  //Dormir durante un tiempo
  Utils.setLED(LED1, LED_OFF);
  Utils.blinkRedLED(1000, 3);
  PWR.deepSleep("00:00:26:55",RTC_OFFSET, RTC_ALM1_MODE4, ALL_OFF);

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

void setupON(){
  USB.ON();
  CO2.ON();
  //ACC.ON()
  xbeeZB.ON();
  checkNetworkParams();
  //Configurar Frame
  frame.createFrame(ASCII,WASPMOTE_ID);
  frame.setFrameSize(92);
}

