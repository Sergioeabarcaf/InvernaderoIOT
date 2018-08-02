/***************************************************
  Adafruit MQTT Library FONA Example

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963
  ----> http://www.adafruit.com/products/2468
  ----> http://www.adafruit.com/products/2542

  These cellular modules use TTL Serial to communicate, 2 pins are
  required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

 ****************************************************/
#include "Adafruit_SleepyDog.h"
#include "SoftwareSerial.h"
#include "Adafruit_FONA.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_FONA.h"
#include "DHT.h"
#include "OneWire.h"
#include "DallasTemperature.h"

/*
  # Sensor DFrobot moisture sensor
  # 0  ~300     seco
  # 300~700     humedo
  # 700~950     en agua

*/

/*************************Geolocalizacion ********************************/
float latitude, longitude, speed_kph, heading, altitude;

/*************************Bater�a********************************/
uint16_t vbat;

/*************************** FONA Pins ***********************************/

/***
  FONA 808               ARDUINO UNO
  GND -----------------> GND
  VIO <----------------- 5V
  RX  <----------------- 2 (TX)
  TX  -----------------> 3 (RX)
  RTS -----------------> 4 GPIO
  KEY -----------------> GND */

// Definicion pins FONA
#define FONA_RX  2
#define FONA_TX  3
#define FONA_RST 4

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);


///************************Sensor de humedad DHT11*****************************/
#define DHTPIN    7  //pin de conexi�n
#define DHTTYPE   DHT11 //tipo aplicado

///***********************Moiseture Sensor DFrobot*************************************/
#define MoistureSensor  A0

///***********************Sensor Temperatura resistente al agua************************/
#define SensorTempAgua 6

/*************************** Inicializaci�n DHT ************************/
DHT dht (DHTPIN, DHTTYPE);
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

/************************Inicializacion Sensor Temperatura Resistente al Agua****/
OneWire oneWireObjeto(SensorTempAgua);
DallasTemperature sensorDS18B20(&oneWireObjeto);
/************************* WiFi Access Point *********************************/

// Optionally configure a GPRS APN, username, and password.
// You might need to do this to access your network's GPRS/data
// network.  Contact your provider for the exact APN, username,
// and password values.  Username and password are optional and
// can be removed, but APN is required.
#define FONA_APN       "imovil.entelpcs.cl"
#define FONA_USERNAME  "entelpcs"
#define FONA_PASSWORD  "entelpcs"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "idiProteinlab"
#define AIO_KEY         "c905c763e9d444b28a3051b0a57d10b9"

/************ Global State (you don't need to change this!) ******************/

// Setup the FONA MQTT class by passing in the FONA class and MQTT server and login details.
Adafruit_MQTT_FONA mqtt(&fona, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }

// FONAconnect is a helper function that sets up the FONA and connects to
// the GPRS network. See the fonahelper.cpp tab above for the source!
boolean FONAconnect(const __FlashStringHelper *apn, const __FlashStringHelper *username, const __FlashStringHelper *password);

/****************************** Feeds ***************************************/

uint8_t txFailures = 0;       // Count of how many publish failures have occured in a row.

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
/**** DHT22: temperatura ****/
Adafruit_MQTT_Publish dht_temp_feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dht22_temp");

/**** DHT22: sensaci�n t�rmica (indicador de calor) ****/
//Adafruit_MQTT_Publish dht_heat_feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dht22_heat");

/**** DHT22: humedad ****/
Adafruit_MQTT_Publish dht_hum_feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dht22_hum");

/**** Moisture Sensor DFrobot ***/
Adafruit_MQTT_Publish moisture_sensor_feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hc");

/*****Sensor Temperatura Resistente al Agua****/
Adafruit_MQTT_Publish sensor_temp_agua_feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tc");

/**** MAPA: desde GPS ****/
Adafruit_MQTT_Publish gpsloc = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gpsloc/csv");

/**** MAPA: desde GSM ****/
Adafruit_MQTT_Publish gsmloc = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gsmloc/csv");

/**** Battery ****/
Adafruit_MQTT_Publish nivel_bateria = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/nivel_bat");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// How many transmission failures in a row we're willing to be ok with before reset
uint8_t txfailures = 0;
#define MAXTXFAILURES 3
//unsigned long startMillis;
//unsigned long currentMillis;
//const unsigned long period = 1800000;  //the value is a number of milliseconds, ie 1 second
void setup() {

  delay(2000);    //Estabilizaci�n del sistema, requerido para DHT22
  dht.begin();
  sensorDS18B20.begin();
//  startMillis = millis();
  while (!Serial);

  // Watchdog is optional!
  //Watchdog.enable(8000);

  Serial.begin(115200);

  Serial.println(F("Adafruit FONA MQTT demo"));

  mqtt.subscribe(&onoffbutton);

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  Watchdog.reset();

  // Initialise the FONA module
  while (! FONAconnect(F(FONA_APN), F(FONA_USERNAME), F(FONA_PASSWORD))) {
    Serial.println("Retrying FONA");
  }

  Serial.println(F("Connected to Cellular!"));

  //Enable GPS
  fona.enableGPS(true);

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  Watchdog.reset();
}

uint32_t x = 0;   //usado para contar

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}


/****desde fonahelper.cpp******/
boolean FONAconnect(const __FlashStringHelper *apn, const __FlashStringHelper *username, const __FlashStringHelper *password) {
  Watchdog.reset();

  Serial.println(F("Initializing FONA....(May take 3 seconds)"));

  fonaSS.begin(4800); // if you're using software serial

  if (! fona.begin(fonaSS)) {           // can also try fona.begin(Serial1)
    Serial.println(F("Couldn't find FONA"));
    return false;
  }
  fonaSS.println("AT+CMEE=2");
  Serial.println(F("FONA is OK"));
  Watchdog.reset();
  Serial.println(F("Checking for network..."));
  while (fona.getNetworkStatus() != 1) {
    delay(500);
  }

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  Watchdog.reset();

  fona.setGPRSNetworkSettings(apn, username, password);

  Serial.println(F("Disabling GPRS"));
  fona.enableGPRS(false);

  Watchdog.reset();
  delay(5000);  // wait a few seconds to stabilize connection
  Watchdog.reset();

  Serial.println(F("Enabling GPRS"));
  if (!fona.enableGPRS(true)) {
    Serial.println(F("Failed to turn GPRS on"));
    return false;
  }
  Watchdog.reset();

  return true;
}


/*********Porcentaje de Humedad, m�dulo DHT22 ***********/
void logHumedad(uint32_t indicator, Adafruit_MQTT_Publish& publishFeed) {

  // Publish
  Serial.print(F("Publishing DHT22 humedad: "));
  Serial.println(indicator);
  if (!publishFeed.publish(indicator)) {
    Serial.println(F("Publish failed!"));
    txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    txFailures = 0;
  }

}

/*********Porcentaje de Temperatura, m�dulo DHT22 ***********/
void logTemperatura(uint32_t indicator, Adafruit_MQTT_Publish& publishFeed) {

  // Publish
  Serial.print(F("Publishing DHT22 temperatura: "));
  Serial.println(indicator);
  if (!publishFeed.publish(indicator)) {
    Serial.println(F("Publish failed!"));
    txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    txFailures = 0;
  }

}
/*****************Porcentaje de Humedad de suelo ******************************/
void logHumedadSueloPercent(uint32_t indicator, Adafruit_MQTT_Publish& publishFeed) {

  // Publish
  Serial.print(F("Publishing Humedad de suelo: "));
  Serial.println(indicator);
  if (!publishFeed.publish(indicator)) {
    Serial.println(F("Publish failed!"));
    txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    txFailures = 0;
  }

}

/****************Sensor Temperatura Resistente al Agua ******************************/
void logTemperaturaContacto(uint32_t indicator, Adafruit_MQTT_Publish& publishFeed) {

  // Publish
  Serial.print(F("Publishing Temperatura de contacto: "));
  Serial.println(indicator);
  if (!publishFeed.publish(indicator)) {
    Serial.println(F("Publish failed!"));
    txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    txFailures = 0;
  }

}
/*********Porcentaje de Bater�a, m�dulo FONA ***********/
void logBatteryPercent(uint32_t indicator, Adafruit_MQTT_Publish& publishFeed) {

  // Publish
  Serial.print(F("Publishing battery percentage: "));
  Serial.println(indicator);
  if (!publishFeed.publish(indicator)) {
    Serial.println(F("Publish failed!"));
    txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    txFailures = 0;
  }

}


// Serialize the lat, long, altitude to a CSV string that can be published to the specified feed.
void logLocationGPS(float latitude, float longitude, float altitude, Adafruit_MQTT_Publish& publishFeed) {
  // Initialize a string buffer to hold the data that will be published.
  char sendBuffer[120];
  memset(sendBuffer, 0, sizeof(sendBuffer));
  int index = 0;

  // Start with '0,' to set the feed value.  The value isn't really used so 0 is used as a placeholder.
  sendBuffer[index++] = '0';
  sendBuffer[index++] = ',';

  // Now set latitude, longitude, altitude separated by commas.
  dtostrf(latitude, 2, 6, &sendBuffer[index]);
  index += strlen(&sendBuffer[index]);
  sendBuffer[index++] = ',';
  dtostrf(longitude, 3, 6, &sendBuffer[index]);
  index += strlen(&sendBuffer[index]);
  sendBuffer[index++] = ',';
  dtostrf(altitude, 2, 6, &sendBuffer[index]);

  // Finally publish the string to the feed.
  Serial.print(F("Publishing location: "));
  Serial.println(sendBuffer);
  if (!publishFeed.publish(sendBuffer)) {
    Serial.println(F("Publish failed!"));
    txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    txFailures = 0;
  }
}


// Serialize the lat, long, altitude to a CSV string that can be published to the specified feed.
void logLocationGSM(float latitude, float longitude, Adafruit_MQTT_Publish& publishFeed) {
  // Initialize a string buffer to hold the data that will be published.
  char sendBuffer[120];
  memset(sendBuffer, 0, sizeof(sendBuffer));
  int index = 0;

  // Start with '0,' to set the feed value.  The value isn't really used so 0 is used as a placeholder.
  sendBuffer[index++] = '1';
  sendBuffer[index++] = ',';

  // Now set latitude, longitude, altitude separated by commas.
  dtostrf(latitude, 2, 6, &sendBuffer[index]);
  index += strlen(&sendBuffer[index]);
  sendBuffer[index++] = ',';
  dtostrf(longitude, 3, 6, &sendBuffer[index]);
  index += strlen(&sendBuffer[index]);
  sendBuffer[index++] = ',';
  dtostrf(altitude, 2, 6, &sendBuffer[index]);

  // Finally publish the string to the feed.
  Serial.print(F("Publishing location: "));
  Serial.println(sendBuffer);
  if (!publishFeed.publish(sendBuffer)) {
    Serial.println(F("Publish failed!"));
    txFailures++;
  }
  else {
    Serial.println(F("Publish succeeded!"));
    txFailures = 0;
  }
}


void loop() {
  // Make sure to reset watchdog every loop iteration!
//  currentMillis = 1800000;
//  if ((currentMillis - startMillis)>= period)
//  {
    Watchdog.reset();

    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    MQTT_connect();
    Watchdog.reset();

    //////////////////////////////////////////////////////////////////////////
    //Adquirimos geolocalizaci�n del m�dulo GPS
    float latitude, longitude, speed_kph, heading, altitude;
    bool gpsFix = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);
    Watchdog.reset();

    //  //Adquirimos geolocalizaci�n del m�dulo GSM_GPRS
    float latitude_gsm, longitude_gsm;
    bool gsmloc_success = fona.getGSMLoc(&latitude_gsm, &longitude_gsm);
    Watchdog.reset();

    //nivel bater�a
    fona.getBattPercent(&vbat);
    Watchdog.reset();

    //nivel de humedad en DHT22;
    float h = dht.readHumidity();
    Watchdog.reset();

    //nivel de temperatura en DHT22:
    float t_c = dht.readTemperature();    //por defecto se mide en grados Celsius
    Watchdog.reset();

    //nivel de humedad de suelo:
    float hc = (analogRead(MoistureSensor) * 100) / 950;
    Watchdog.reset();

    //nivel de temperatura de contacto
    sensorDS18B20.requestTemperatures();
    delay(2000);
    Watchdog.reset();
    float tc = sensorDS18B20.getTempCByIndex(0);
    Watchdog.reset();
    //nivel de sensaci�n t�rmica (heat-indicator), medido en �C
    //float hic = dht.computeHeatIndex(t_c, h);
    //Watchdog.reset();

    //publicamos los datos

    logHumedad(h, dht_hum_feed);
    logTemperatura(t_c, dht_temp_feed);
    logHumedadSueloPercent(hc, moisture_sensor_feed);
    logTemperaturaContacto(tc, sensor_temp_agua_feed);
    //  logTemperatura(hic, dht_heat_feed);
    logLocationGPS(latitude, longitude, altitude, gpsloc);
    logLocationGSM(latitude_gsm, longitude_gsm, gsmloc);
    logBatteryPercent(vbat, nivel_bateria);
    delay(1800000);

    //recibimos los datos esperados desde el publisher
    // this is our 'wait for incoming subscription packets' busy subloop
    // Adafruit_MQTT_Subscribe *subscription;
    //  while ((subscription = mqtt.readSubscription(5000))) {
    //    if (subscription == &onoffbutton) {
    //      Serial.print(F("Got: "));
    //      Serial.println((char *)onoffbutton.lastread);
//  startMillis = currentMillis;}
} //fin_loop()

