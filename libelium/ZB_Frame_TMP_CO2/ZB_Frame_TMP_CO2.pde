/*
 * Obtener Temperatura, Humedad y Presión
 * Programa desarrollado en PROTEINLAB por Sergio Abarca F 
 */


//====================================================================
//      Librerias a utilizar

#include <WaspSensorGas_v30.h>
#include <WaspXBeeZB.h>
#include <WaspFrame.h>

//====================================================================
//      Configuracion ZB y Frame

//MAC de GW Digi
char RX_ADDRESS[] = "0013A20040DC588F";

//ID que usara
char WASPMOTE_ID[] = "WaspTHP_CO2";

//Variables a usar
uint8_t error;

//====================================================================
//      Configuración Temp, hum y press

//Temp, Hum, Press
float t,h,p;

//====================================================================
//      Configuración de CO2

//Inicializar CO2
CO2SensorClass CO2Sensor;
// Concentratios used in calibration process (PPM Values)
#define POINT1_PPM_CO2 350.0  //   <-- Normal concentration in air
#define POINT2_PPM_CO2 1000.0
#define POINT3_PPM_CO2 3000.0

// Calibration vVoltages obtained during calibration process (Volts)
#define POINT1_VOLT_CO2 0.300
#define POINT2_VOLT_CO2 0.350
#define POINT3_VOLT_CO2 0.380

#define numPoints 3

float concentrations[] = { POINT1_PPM_CO2, POINT2_PPM_CO2, POINT3_PPM_CO2 };
float voltages[] =       { POINT1_VOLT_CO2, POINT2_VOLT_CO2, POINT3_VOLT_CO2 };

//====================================================================

void setup()
{
  USB.ON();
  USB.println("Obtener Temperatura, Humedad, Presion y CO2 desde Sensor Board V3");
  USB.println("Enviar Frame con datos obtenidos via ZB.");
  
  //Calibrar Sensor
  CO2Sensor.setCalibrationPoints(voltages, concentrations, numPoints);

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

  // Obtener CO2 para agregar a Frame
  CO2Sensor.ON();
  delay(500);

  frame.addSensor(SENSOR_GASES_CO2,CO2Sensor.readConcentration());

  CO2Sensor.OFF();
  delay(500);

  // Obtener Temp, Hum y Press para agregar a Frame
  Gases.ON();
  delay(500);

  frame.addSensor(SENSOR_GASES_PRO_TC,Gases.getTemperature());
  frame.addSensor(SENSOR_GASES_PRO_HUM,Gases.getHumidity());
  frame.addSensor(SENSOR_GASES_PRO_PRES,Gases.getPressure());

  Gases.OFF();
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

  // Dormir por 5 segundo
  USB.println("Me voy a dormir una pequeña siesta :). Ya vengo! ");
  PWR.deepSleep("00:00:00:05", RTC_OFFSET, RTC_ALM1_MODE1, ALL_OFF);
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
