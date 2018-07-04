/*
 * Obtener Temperatura, Humedad y Presión
 * Programa desarrollado en PROTEINLAB por Sergio Abarca F
 */

#include <WaspSensorGas_v30.h>
#include <WaspXBeeZB.h>
#include <WaspFrame.h>

//MAC de GW Digi
char RX_ADDRESS[] = "0013A20040DC588F";

//ID que usara
char WASPMOTE_ID[] = "WaspCO2";

//Variables a usar
uint8_t error;


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

void setup()
{
  USB.ON();
  delay(3000);
  USB.println("Obtener CO2 desde Sensor Board V3 y enviar en Frame a ZB");
  
  //Configurar ID de Frame
  frame.setID( WASPMOTE_ID );
}


void loop()
{
  // Dormir por 10 minutos
  USB.println("Me voy a dormir una pequeña siesta de 20 minuto :) Ya vengo! ");
  // http://www.libelium.com/downloads/documentation/waspmote-interruptions-programming_guide.pdf pag:12
  // http://www.libelium.com/downloads/documentation/waspmote-rtc-programming_guide.pdf
  PWR.deepSleep("00:00:20:00", RTC_OFFSET, RTC_ALM1_MODE2, ALL_OFF);
  //Inicio de parametros necesarios y configuraciones
  USB.ON();
  delay(3000);
  USB.println("Ya desperte!!!!, ahora mandare el dato del CO2... calmacion, necesito configurarme");
  //Activar red y verificar conexion
  xbeeZB.ON();
  checkNetworkParams();
  //Calibrar Sensor
  CO2Sensor.setCalibrationPoints(voltages, concentrations, numPoints);
  Gases.ON(); 
  CO2Sensor.ON();

  //Ver parametros del sensor CO2
  float CO2Vol = CO2Sensor.readVoltage();
  float CO2PPM = CO2Sensor.readConcentration();
  USB.print(F("CO2 Sensor Voltage: "));
  USB.print(CO2Vol);
  USB.print(F("volts |"));
  USB.print(F(" CO2 concentration estimated: "));
  USB.print(CO2PPM);
  USB.println(F(" ppm"));

  //Creacion de Frame en modo ASCII
  frame.createFrame(ASCII);
  frame.setFrameSize(92);
  frame.addSensor(SENSOR_GASES_CO2, CO2PPM);
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
}

// Revisar parametros de conexion
void checkNetworkParams()
{
  xbeeZB.getOperating64PAN();
  xbeeZB.getAssociationIndication();
  while ( xbeeZB.associationIndication != 0 )
  {
    USB.print(".");
    delay(2000);
    xbeeZB.getAssociationIndication();
  }
  USB.println(F("\nConectado en la red"));
}
