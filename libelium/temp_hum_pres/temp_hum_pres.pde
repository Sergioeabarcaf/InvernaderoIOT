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
  delay(1000);
}


void loop()
{
  Agriculture.ON();
  delay(500);
  
  t = Agriculture.getTemperature();
  h  = Agriculture.getHumidity();
  p = Agriculture.getPressure();

  Agriculture.OFF();
  
  USB.print("Temperatura en grados celsius: ");
  USB.println(t);
  USB.print("Humedad relativa: ");
  USB.println(h);
  USB.print("Presión atmosferica en Pa: ");
  USB.println(p);

  // Dormir por 3 minutos
  USB.println("Me voy a dormir una pequeña siesta :). Ya vengo! ");
  PWR.deepSleep("00:00:03:00", RTC_OFFSET, RTC_ALM1_MODE1, ALL_OFF);
  USB.ON();
  USB.println("Ya desperte!!!!");
  
}
