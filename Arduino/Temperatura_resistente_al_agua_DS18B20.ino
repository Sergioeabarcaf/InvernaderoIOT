#include <OneWire.h>
#include <DallasTemperature.h>

const int pinsensor = 9;
// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinsensor);
DallasTemperature sensorDS18B20(&oneWireObjeto);
void setup() {
  Serial.begin(9600);
  Serial.println("Sensor de temperatura DS18B20");
  sensorDS18B20.begin();
}

void loop() {
  sensorDS18B20.requestTemperatures();
  Serial.print("Temperatura : ");
  Serial.print(sensorDS18B20.getTempCByIndex(0));
  Serial.println(" C");
  //Se demora en actualizar la temperatura
  delay(5000);
}
