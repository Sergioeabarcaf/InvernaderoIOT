/*
  Obtencion de CO2 siguiendo la documentacion de Gases PRO v30 Board Guide
  http://www.libelium.com/downloads/documentation/gases_sensor_board_pro.pdf

  
*/
#include <WaspSensorGas_Pro.h>

Gas CO2(SOCKET_1);
float temperatura, humedad, presion, concentracionCO2;
int errorCO2;

void setup()
{
  USB.ON();
  USB.println("Estoy tratandi de encender");
  errorCO2 = CO2.ON();
  USB.println(errorCO2);
  USB.println("Ese fue el error");
  delay(120000);
  // put your setup code here, to run once:

}


void loop()
{
  temperatura = CO2.getTemp(0);
  USB.print("AFE: ");
  USB.println(temperatura);
  temperatura = CO2.getTemp(1);
  USB.print("BME280: ");
  USB.println(temperatura);
  humedad = CO2.getHumidity();
  USB.print("humedad: ");
  USB.println(humedad);
  presion = CO2.getPressure();
  USB.print("presion: ");
  USB.println(presion);
  concentracionCO2 = CO2.getConc();
  USB.print("CO2: ");
  USB.println(concentracionCO2);
  USB.println("=======================================================");
  delay(5000);
  // put your main code here, to run repeatedly:

}
