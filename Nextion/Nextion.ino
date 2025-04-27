
#include "NextionSoftSerial.h"                   // ha sido modificado para aceptar Serial2 Hardware ESP32
#include "NextionObject.h"

/********************************************************/
/* void setup()                                         */
/********************************************************/
void setup()
{

char cmd[100];

  Serial.begin(115200);
   

  //-- INI NEXTION DISPLAY -------------------------------------------------------------------
  Serial2.begin(19200);    // Inicializa puerto sere Nextion DISPLAY
  initNEXTION();           // Inicializa Nextion DISPLAY
  delay(250);
  sendNEXTIONcmd("rest");
  delay(1500);
     
  sendNEXTIONcmd("page 0");
  strcpy(cmd,"\r\nSYSTEM> Initializing ... Nextion_SoftSerial");
  Serial.println(cmd); 
  sendNEXTIONcmd("xstr 0,18,390,18,1,RED,WHITE,0,1,1,\"SYSTEM> Initializing ... Nextion_SoftSerial\"");
  delay(1500);
  
}


/********************************************************/
/* void loop()                                          */
/********************************************************/
void loop()
{// ... loop
String ev;
char cmd[100];

       ev=listenNEXTION();
       if (ev != "" )
       {
         Serial.print("\r\nTFT> ev=");
         Serial.println(ev.c_str());

         if (ev==bStart) 
         {
            strcpy(cmd,"\r\nSYSTEM> bStart push");
            Serial.println(cmd);

            sendNEXTIONcmd("xstr 0,18,390,18,1,RED,WHITE,0,1,1,\"SYSTEM> ... bStart push\"");
         }
       }
       delay(100);
       

 
}// ... loop
