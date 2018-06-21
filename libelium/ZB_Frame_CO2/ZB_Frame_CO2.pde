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
  xbeeZB.ON();
  USB.println("Obtener CO2 desde Sensor Board V3 y enviar en Frame a ZB");

   //Configurar ID de Frame
  frame.setID( WASPMOTE_ID );

  delay(3000);
  //Calibrar Sensor
  CO2Sensor.setCalibrationPoints(voltages, concentrations, numPoints);
  CO2Sensor.ON();
}


void loop()
{
  //Creacion de Frame en modo ASCII
  frame.createFrame(ASCII);
  frame.setFrameSize(92);
  frame.addSensor(SENSOR_GASES_CO2, CO2Sensor.readConcentration());
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

  // Espera de 1800000 segundos (30 minutos) para un nuevo envio de Frame
  delay(1800000);
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
