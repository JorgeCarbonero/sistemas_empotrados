#ifndef __NEXTION_H__

  #define __NEXTION_H__

  // NEXTION CODES
  #define NEX_RET_CMD_FINISHED                (0x01)
  #define NEX_RET_EVENT_LAUNCHED              (0x88)
  #define NEX_RET_EVENT_UPGRADED              (0x89)
  #define NEX_RET_EVENT_TOUCH_HEAD            (0x65)     
  #define NEX_RET_EVENT_POSITION_HEAD         (0x67)
  #define NEX_RET_EVENT_SLEEP_POSITION_HEAD   (0x68)
  #define NEX_RET_CURRENT_PAGE_ID_HEAD        (0x66)
  #define NEX_RET_STRING_HEAD                 (0x70)
  #define NEX_RET_NUMBER_HEAD                 (0x71)
  #define NEX_RET_INVALID_CMD                 (0x00)
  #define NEX_RET_INVALID_COMPONENT_ID        (0x02)
  #define NEX_RET_INVALID_PAGE_ID             (0x03)
  #define NEX_RET_INVALID_PICTURE_ID          (0x04)
  #define NEX_RET_INVALID_FONT_ID             (0x05)
  #define NEX_RET_INVALID_BAUD                (0x11)
  #define NEX_RET_INVALID_VARIABLE            (0x1A)
  #define NEX_RET_INVALID_OPERATION           (0x1B)
  
  #include <Arduino.h>
   
  // RX y TX deben estar cruzados
  #define pinTX 16
  #define pinRX 17  
  
  bool initNEXTION(const char* pageId ="0");              
  void sendNEXTIONcmd(const char* cmd);             	  
  String listenNEXTION(void);            
  String evNEXTION(void); 
  
#endif
