#include <XBee.h>

//Objeto XBee

XBee xbee = XBee();

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);

uint8_t payload[] = {0,0};
ZBTxRequest mensaje = ZBTxRequest(addr64, payload, sizeof(payload));


void setup() {

  Serial.begin(9600);
  xbee.setSerial(Serial);
  

}

void loop() {

payload[0] = 2 & 0xff;;
payload[1]= 5 & 0xff;
xbee.send(mensaje);
delay(1000);
 }


