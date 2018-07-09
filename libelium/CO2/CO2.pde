/*
  Obtencion de CO2 siguiendo la documentacion de Gases PRO v30 Board Guide
  http://www.libelium.com/downloads/documentation/gases_sensor_board_pro.pdf

  
*/
#include <WaspSensorGas_Pro.h>

Gas CO2(SOCKET_1);
float temperatura, humedad, presion, concentracionCO2;
int intentoEncender = 0;

void setup()
{
  USB.ON();
  USB.println("Estoy tratando de encender");
  if(CO2.ON() != 1){
    USB.println("error es -1");
    intentoEncender++;
  }
  else{
    USB.print("Se intento: ");
    USB.println(intentoEncender);
    USB.println("Encendio bien, esperar 120 segundos para calentar sensor");
    delay(120000);
  }
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

}
