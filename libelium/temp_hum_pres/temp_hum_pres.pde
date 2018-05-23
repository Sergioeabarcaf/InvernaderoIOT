/*
 * Obtener Temperatura, Humedad y Presión
 * Programa desarrollado en PROTEINLAB por Sergio Abarca F 
 */

#include <WaspSensorAgr_v30.h>

float t,h,p;

void setup()
{
  //Inicializacion de objetos
  USB.ON();
  USB.println(F("Obtener Temperatura, Humedad y Presión"));
  Agriculture.ON();
  delay(3000);
}


void loop()
{
  t = Agriculture.getTemperature();
  h  = Agriculture.getHumidity();
  p = Agriculture.getPressure();

  
  USB.print("Temperatura en grados celsius: ");
  USB.println(t);
  USB.print("Humedad relativa: ");
  USB.println(h);
  USB.print("Presión atmosferica en Pa: ");
  USB.println(p);


  // Pausa de 60 segundos.
  delay(60000);
  
}
