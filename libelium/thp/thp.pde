/*
 * Programa desarrollado por Sergio Abarca F
 * 
  Uso de Frame
  http://www.libelium.com/downloads/documentation/data_frame_guide.pdf
  Uso de acelerometro
  http://www.libelium.com/downloads/documentation/waspmote-accelerometer-programming_guide.pdf
  
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
char WASPMOTE_ID[] = "WTHP";
//Variables a usar
uint8_t error;
//====================================================================

void setup() { }


void loop()
{
  Utils.setLED(LED1, LED_ON);
  PWR.deepSleep("00:00:01:00",RTC_OFFSET, RTC_ALM1_MODE4, ALL_ON);
  setupON();
  
  //Cargar parametros al frame
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  //frame.addSensor(SENSOR_ACC, ACC.getX(),ACC.getY(),ACC.getZ());
  frame.addSensor(SENSOR_AGR_TC, Agriculture.getTemperature());
  frame.addSensor(SENSOR_AGR_HUM, Agriculture.getHumidity());
  frame.addSensor(SENSOR_AGR_PRES, Agriculture.getPressure());

  //Mostrar Frame
  frame.showFrame();
  
  //Enviar
  error = xbeeZB.send(RX_ADDRESS, frame.buffer, frame.length);
  // Comprobar que el envio sea correcto
  if(error == 0){
    USB.println("Envio OK");
    Utils.blinkGreenLED(330, 3);
  }
  else{
    Utils.blinkLEDs(30000);
    USB.print("ERROR: ");
    USB.println(error);
  }
  
  //Dormir durante un tiempo
  Utils.setLED(LED1, LED_OFF);
  Utils.blinkRedLED(1000, 3);
  PWR.deepSleep("00:00:28:55",RTC_OFFSET, RTC_ALM1_MODE4, ALL_OFF);

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
  //Inicializador de objetos
  USB.ON();
  Agriculture.ON();
  //ACC.ON();
  xbeeZB.ON();
  //Verificar la conexion a la red
  checkNetworkParams();
  //Configurar Frame
  frame.createFrame(ASCII,WASPMOTE_ID);
  frame.setFrameSize(92);
}

