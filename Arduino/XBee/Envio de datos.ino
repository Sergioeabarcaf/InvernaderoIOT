#include <Printers.h>
#include <XBee.h>

//Objeto XBee

XBee xbee = XBee();

// SH + SL Address of receiving XBee
uint8_t payload[] = {0,0 };
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40dc588f);
ZBTxRequest mensaje = ZBTxRequest(addr64, payload, sizeof(payload));


void setup() {
  delay(1000);
  Serial.begin(9600);
  xbee.setSerial(Serial);
  xbee.send(mensaje);

}

void loop() {

 }


