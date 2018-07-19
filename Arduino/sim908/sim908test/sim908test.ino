int data_size;
char data[512];
char aux;
int x = 0;
int8_t answer;
char aux_str[100];


void setup()
 {
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
   //Output GSM Timing 
    digitalWrite(5,HIGH);
    delay(1500);
    digitalWrite(5,LOW); 
    Serial.begin(9600);
     
    digitalWrite(3,LOW);//enable GSM TX、RX
    digitalWrite(4,HIGH);//disable GPS TX、RX
    
    delay(20000);
    start_GSM();
 }
 
 void loop(){ 
   send_GPRS();
   delay(30000);
 }
 
 void start_GSM(){
    Serial.println("AT");
    delay(2000);
    Serial.println("AT+CREG?");
    delay(2000);
//  Operador Entel
    Serial.println("AT+SAPBR=3,1,\"APN\",\"imovil.entelpcs.cl\"");
    delay(2000);
    Serial.println("AT+SAPBR=3,1,\"USER\",\"entelpcs\"");
    delay(2000);
    Serial.println("AT+SAPBR=3,1,\"PWD\",\"entelpcs\"");
    delay(2000);
    Serial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    delay(2000);
    Serial.println("AT+SAPBR=1,1");
    delay(10000);
    Serial.println("AT+CIICR");
    delay(300);
    Serial.println("AT+HTTPINIT");
    delay(2000);
    Serial.println("AT+HTTPPARA=\"CID\",1");
    delay(2000);
 }
 
 void send_GPRS(){
    digitalWrite(3,LOW);//Habilita GSM mode
    digitalWrite(4,HIGH);//Deshabilitida GPS mode
    Serial.print("AT+HTTPPARA=\"URL\",\"https://libelium-91af3.firebaseio.com/sim908");
    delay(2000);
    Serial.println("AT+HTTPACTION=1"); //post
    delay(10000);
    Serial.println("AT+CIPSEND");
    delay(4500);
    Serial.println("Hola mundo");
    

    delay(500);
    Serial.println("AT+CIPCLOSE");//close the connection
 
   

    //sendATcommand("AT+HTTPTERM", "OK", 5000);
    //delay(5000);
}
  
int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Inicilizar String
    delay(100);

    while( Serial.available() > 0) Serial.read();    // Limpio

    Serial.println(ATcommand);    // Envio AT command


        x = 0;
    previous = millis();

    //Esperar por la respuesta
    do{
        if(Serial.available() != 0){    
            response[x] = Serial.read();
            x++;
            // Chequeo de la respuesta para ver si esta dentro de los rangos esperados
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
        }
    }
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}



int8_t sendATcommand2(char* ATcommand, char* expected_answer1, 
char* expected_answer2, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    

    delay(100);

    while( Serial.available() > 0) Serial.read();    

    Serial.println(ATcommand);   


        x = 0;
    previous = millis();


    do{        
        if(Serial.available() != 0){    
            response[x] = Serial.read();
            x++;

            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }

            if (strstr(response, expected_answer2) != NULL)    
            {
                answer = 2;
            }
        }

    }while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}

void sendDatatoServer()
{
  
}

