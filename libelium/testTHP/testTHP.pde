// Visualizacion de Temperatura, Humedad y Presion en pantalla para pruebas
// Programa desarrollado en PROTEINLAB por Sergio Abarca F.

#include <WaspSensorAgr_v30.h>

//Variables a usar
uint8_t error;


void setup()
{
  //Inicializacion de objetos
  USB.ON();
  USB.println(F("Visualizar Temperatura, Humedad y Presion SIN CASE cada 1 minuto."));
  Agriculture.ON();

  delay(2000);
}


void loop()
{
  USB.print("Temperatuta: ");
  USB.println(Agriculture.getTemperature());
  USB.print("Humedad: ");
  USB.println(Agriculture.getHumidity());
  USB.print("Presion; ");
  USB.println(Agriculture.getPressure());
  USB.println("==============================");

  // Espera de 60 segundos para hacer una lectura
  delay(60000);
}
