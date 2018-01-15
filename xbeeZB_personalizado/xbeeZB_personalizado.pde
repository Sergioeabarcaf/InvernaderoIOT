
//Librerias

#include <WaspXBeeZB.h>

//Variables

uint8_t panID[8] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
uint8_t channel = 0x13;

void setup()
{
  USB.ON();
  xbeeZB.ON();

  xbeeZB.getOwnMacLow();

  xbeeZB.setPAN(panID);
  xbeeZB.getPAN();

  //Almacenar los cambios para cuando se reinicie.
  //xbeeZB.writeValues();

  USB.println("Setup terminado");
}


void loop()
{
  // put your main code here, to run repeatedly:
}

