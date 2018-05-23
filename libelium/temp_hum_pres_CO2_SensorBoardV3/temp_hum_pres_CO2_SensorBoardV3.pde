/*
 * Obtener Temperatura, Humedad y Presión
 * Programa desarrollado en PROTEINLAB por Sergio Abarca F 
 */

#include <WaspSensorGas_v30.h>

//Temp, Hum, Press
float t,h,p;


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
  USB.println("Obtener Temperatura, Humedad, Presion y CO2 desde Sensor Board V3");
  //Calibrar Sensor
  CO2Sensor.setCalibrationPoints(voltages, concentrations, numPoints);
  Gases.ON();
  CO2Sensor.ON();

  delay(1000);
}


void loop()
{
  t = Gases.getTemperature();
  h  = Gases.getHumidity();
  p = Gases.getPressure();

  USB.println("=================================");
  
  USB.print("Temperatura en grados celsius: ");
  USB.println(t);
  USB.print("Humedad relativa: ");
  USB.println(h);
  USB.print("Presión atmosferica en Pa: ");
  USB.println(p);

  USB.println("=================================");

  float CO2Vol = CO2Sensor.readVoltage();
  float CO2PPM = CO2Sensor.readConcentration();

  USB.print("Voltaje sensor en Volt: ");
  USB.println(CO2Vol);
  
  USB.print("Concentracion CO2 estimada en PPM: ");
  USB.println(CO2PPM);

  USB.println("=================================");


  delay(2000);
}
