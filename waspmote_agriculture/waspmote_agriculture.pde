
// Put your libraries here (#include ...)

#include <WaspXBeeZB.h>
#include <WaspSensorAgr_v30.h>

//Variables

float temp,hume,pres;


void setup()
{
  //Inicializacion puerto USB
  USB.ON();
  USB.println("Waspmote iniciado");
  
  // Configuracion Xbee ZB
  xbeeZB.ON();

  //Configuracion placa agriculture
  Agriculture.ON();

}


void loop()
{
  //Obtener valores del sensor en placa agriculture

  temp = Agriculture.getTemperature();
  hume  = Agriculture.getHumidity();
  pres = Agriculture.getPressure(); 

  //Imprimir valores cada 3 segundos
  USB.print(F("Temperature: "));
  USB.print(temp);
  USB.println(F(" Celsius"));
  USB.print(F("Humidity: "));
  USB.print(hume);
  USB.println(F(" %"));  
  USB.print(F("Pressure: "));
  USB.print(pres);
  USB.println(F(" Pa"));  
  USB.println(); 
  delay(3000);

}
