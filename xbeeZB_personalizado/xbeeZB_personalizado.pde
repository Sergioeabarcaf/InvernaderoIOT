
//Librerias

#include <WaspXBeeZB.h>

//Variables

uint8_t panID[8] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
uint8_t channel = 0x13;

void setup()
{
  USB.ON();
  xbeeZB.ON();

  if(!xbeeZB.sendCommandAT("CH#")) 
  {
    USB.print(F("Channel: "));
    USB.printHex(xbeeZB.commandAT[0]);
    USB.println();
  }
  else 
  {
    USB.println("at error");
  }


  //Almacenar los cambios para cuando se reinicie.
  //xbeeZB.writeValues();
}


void loop()
{
  // put your main code here, to run repeatedly:
}

