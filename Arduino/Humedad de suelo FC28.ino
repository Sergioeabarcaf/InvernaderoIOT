/*Obtencion humedad de suelo*/

int sensorHumedad = A0;
void setup() {
  Serial.begin(9600);
  pinMode(sensorHumedad,INPUT);
 
}

void loop() {
/*Se ajustan los valores */
int humedad_suelo =map(analogRead(sensorHumedad), 0, 1023, 100, 0);
/*Los valores obtenidos van desde 0 sumergido en agua, a 1023 en el aire*/

Serial.print("Humedad de suelo : ");
Serial.print(humedad_suelo);
Serial.println("%");
delay(1000);

}
