#include "NextionSoftSerial.h"


/*****************************************************
 * Send command to Nextion.                          *
 *                                                   *
 * @param cmd - the string of command.               *
 ****************************************************/
void sendNEXTIONcmd(const char* cmd)
{
  while (Serial2.available())
  {
	  Serial2.read();
  }//end while
  
  Serial2.print(cmd);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
 
}//end sendNEXTIONcmd


bool initNEXTION( const char* pageId)
{
String page = "page " + String(pageId);

  sendNEXTIONcmd("");
  sendNEXTIONcmd("bkcmd=1"); 
  sendNEXTIONcmd(page.c_str());   
  return true;  
}
  
void Nextion_flushSerial(){
   Serial2.flush();
}//end flush



String evNEXTION(void)
{
char bBYTE=0x00;
char sBYTE[4]={0,0};
char _end = 0xff;//end of file x3
String cmd;
int countEnd = 0;

  while(Serial2.available()>0)
  {
 
      bBYTE = Serial2.read();
      sprintf(sBYTE,"[%02x]",bBYTE);
      cmd += sBYTE;
      
      if(bBYTE == _end) countEnd++;
      if(countEnd == 3)
      { 
        Serial2.flush();
        return cmd;
        break;
      }//end if
      
     
  }//end while
  Serial2.flush();
  cmd=String("");
  return cmd;
 
}//end evNextion


String listenNEXTION()
{
char bBYTE=0x00;
char sBYTE[4]={0,0,0,0};
char _end = 0xff;//end of file x3
String cmd;
int countEnd = 0;

  if(Serial2.available()>0)
  {// available
    
        bBYTE = Serial2.read();
        if ( (bBYTE==NEX_RET_NUMBER_HEAD) || (bBYTE==NEX_RET_EVENT_TOUCH_HEAD ) || (NEX_RET_CURRENT_PAGE_ID_HEAD)  )
        {// hay CMD
          
            cmd ="[";
            sprintf(sBYTE,"%x",bBYTE);
            cmd += sBYTE;
            
            while(Serial2.available()>0)
            {//while
              
          	  delay(5);    	
          	  bBYTE = Serial2.read();
          	  sprintf(sBYTE,"%x",bBYTE);
              cmd += sBYTE;
          	  if(bBYTE == _end) countEnd++;
          	  if(countEnd == 3)break;
             
             }//while             
             cmd +="]";
             
        }// hay CMD
        
	}// available
  
  return cmd;
  
}
