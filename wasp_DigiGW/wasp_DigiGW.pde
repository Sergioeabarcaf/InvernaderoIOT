//Envio de temperatura hacia GW y visualizacion en DIGI REMOTE CLOUD
// Programa desarrollado en PROTEINLAB por Sergio Abarca F.

#include <WaspXBeeZB.h>
#include <WaspFrame.h>
#include <WaspSensorAgr_v30.h>

//MAC de GW Digi
char RX_ADDRESS[] = "0013A20040DC588F";


//ID
char WASPMOTE_ID[] = "Waspmote";


//Variables a usar
uint8_t error;
float temp;
char tempSTR[15];


void setup()
{
  //Inicializacion de objetos
  USB.ON();
  USB.println(F("Enviando temperatura a Coordinador"));
  xbeeZB.ON();
  Agriculture.ON();

  //Configurar ID de Frame
  frame.setID( WASPMOTE_ID );
  
  delay(3000);
  
  checkNetworkParams();
}


void loop()
{
  //USB.println(F("FRAME"));
  //Creacion de Frame en modo ASCII
  //frame.createFrame(ASCII);  
  //frame.setFrameSize(92);

  //Agregar contenido al Frame
  //frame.addSensor(SENSOR_AGR_TC, Agriculture.getTemperature());
  
  //Mostrar Frame como flag
  //frame.showFrame();

  // 1- Enviar Frame a Xbee
  //error = xbeeZB.send( RX_ADDRESS, frame.buffer, frame.length );   

  // 2- Enviar dato obtenido de sensor T° a Xbee
  USB.println(F("STRING"));
  //2.1 - Obtener temperatura y transformarlo a String
  temp = Agriculture.getTemperature();
  Utils.float2String (temp, tempSTR, 3);
  USB.println(tempSTR);
 
  //Enviar dato directo a Xbee
  error = xbeeZB.send( RX_ADDRESS, tempSTR);
  
  // check TX flag
  if( error == 0 )
  {
    USB.println(F("Envio OK"));
    USB.println(" ");
    
    // blink green LED
    Utils.blinkGreenLED();
    
  }
  else 
  {
    USB.println(F("ERROR"));
    USB.println(error);
    USB.println(" ");
    
    // blink red LED
    Utils.blinkRedLED();
  }

  // Espera de 30 segundos
  delay(30000);
}

// Revisar parametros de conexion
void checkNetworkParams()
{
  // 1. get operating 64-b PAN ID
  xbeeZB.getOperating64PAN();

  // 2. wait for association indication
  xbeeZB.getAssociationIndication();
 
  while( xbeeZB.associationIndication != 0 )
  { 
    delay(2000);
    
    // get operating 64-b PAN ID
    xbeeZB.getOperating64PAN();

    USB.print(F("operating 64-b PAN ID: "));
    USB.printHex(xbeeZB.operating64PAN[0]);
    USB.printHex(xbeeZB.operating64PAN[1]);
    USB.printHex(xbeeZB.operating64PAN[2]);
    USB.printHex(xbeeZB.operating64PAN[3]);
    USB.printHex(xbeeZB.operating64PAN[4]);
    USB.printHex(xbeeZB.operating64PAN[5]);
    USB.printHex(xbeeZB.operating64PAN[6]);
    USB.printHex(xbeeZB.operating64PAN[7]);
    USB.println();     
    
    xbeeZB.getAssociationIndication();
  }

  USB.println(F("\nJoined a network!"));

  // 3. get network parameters 
  xbeeZB.getOperating16PAN();
  xbeeZB.getOperating64PAN();
  xbeeZB.getChannel();

  USB.print(F("operating 16-b PAN ID: "));
  USB.printHex(xbeeZB.operating16PAN[0]);
  USB.printHex(xbeeZB.operating16PAN[1]);
  USB.println();

  USB.print(F("operating 64-b PAN ID: "));
  USB.printHex(xbeeZB.operating64PAN[0]);
  USB.printHex(xbeeZB.operating64PAN[1]);
  USB.printHex(xbeeZB.operating64PAN[2]);
  USB.printHex(xbeeZB.operating64PAN[3]);
  USB.printHex(xbeeZB.operating64PAN[4]);
  USB.printHex(xbeeZB.operating64PAN[5]);
  USB.printHex(xbeeZB.operating64PAN[6]);
  USB.printHex(xbeeZB.operating64PAN[7]);
  USB.println();

  USB.print(F("channel: "));
  USB.printHex(xbeeZB.channel);
  USB.println();

}



