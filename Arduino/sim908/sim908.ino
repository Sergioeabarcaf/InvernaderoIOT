#include <GSM.h>

// APN data
#define GPRS_APN       " imovil.entelpcs.cl" // replace your GPRS APN
#define GPRS_LOGIN     "entelpcs"    // replace with your GPRS login
#define GPRS_PASSWORD  "entelpcs" // replace with your GPRS password
#define PINNUMBER "6781"
boolean started = false;
long lastime = millis();
char frame[200];
GSMClient client;
GPRS gprs;
GSM gsmAccess;
void connectNetwork()
{
  Serial.println("Connecting to GSM...");
  bool status = false;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  //while (status == false) {
    if ((gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      status = true;
    } else {
      Serial.print(".");
      delay(1000);
    }
  //}

  Serial.println();
  Serial.println("GSM connected");
}
void setup()
{
  Serial.begin(9600);
  digitalWrite(3,LOW);//Enable GSM mode
  digitalWrite(4,HIGH);//Disable GPS mode

//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
   boolean notConnected = true;
    Serial.begin(9600);
     Serial.println("GSM Shield testing.");
 
}
void loop()
{
  connectNetwork();
  }



