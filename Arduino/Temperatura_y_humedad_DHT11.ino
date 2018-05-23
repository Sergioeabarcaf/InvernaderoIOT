#include <DHT.h>

#define pin 4
#define DHTTYPE DHT11
DHT dht(pin,DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor humedad temperatura DHT11");
  dht.begin();
}
void loop() {
  delay(2000);
  Serial.print("DHT11, \t");
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if(isnan(humedad) || isnan(temperatura))
    Serial.println("Error al leer de Sensor DHT");
   
 Serial.print("Humedad : ");
 Serial.print(humedad);
 Serial.print("%\t Temperatura : ");
 Serial.print(temperatura);
 Serial.println("°C");

}
