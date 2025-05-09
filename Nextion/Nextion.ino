#include "NextionSoftSerial.h"
#include "NextionObject.h"
#include "SimonGame.h"
#include "DodgeGame.h"

// Pines físicos
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int yellowPin = 12;
const int buttonRed = 4;
const int buttonGreen = 5;
const int buttonBlue = 6;
const int buttonYellow = 7;
const int joyX = A3;
const int joyY = A4;
const int buttonJoy = 24;

// SimonGame
const int simonEntradas[4] = {buttonRed, buttonGreen, buttonBlue, buttonYellow};
const int simonSalidas [4] = {redPin,   greenPin,   bluePin,   yellowPin};
SimonGame simon(simonEntradas, simonSalidas);
// DodgeGame
DodgeGame dodge(joyX, joyY);

// Estados de pantalla
String currentPage = "pageHome";

void setup() {
  char cmd[100];

  Serial.begin(115200);

  //-- INI NEXTION DISPLAY -------------------------------------------------------------------
  Serial2.begin(19200);             // Inicializa puerto serie Nextion DISPLAY
  initNEXTION();                    // Inicializa Nextion DISPLAY
  delay(250);
  sendNEXTIONcmd("rest");
  delay(1500);
     
  sendNEXTIONcmd("page 0");
  strcpy(cmd,"\r\nSYSTEM> Initializing ... Nextion_SoftSerial");
  Serial.println(cmd); 
  //sendNEXTIONcmd("xstr 0,18,390,18,1,RED,WHITE,0,1,1,\"SYSTEM> Initializing ... Nextion_SoftSerial\"");
  delay(1500);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(yellowPin, OUTPUT);

  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonBlue, INPUT_PULLUP);
  pinMode(buttonYellow, INPUT_PULLUP);

  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);

}

void loop() {
  String ev = listenNEXTION();
  char cmd[100];

  if (ev != "") {
    Serial.print("Evento recibido: ");
    Serial.println(ev.c_str());
    
    if (ev==bStart) 
    {
      strcpy(cmd,"\r\nSYSTEM> bStart push");
      Serial.println(cmd);

      //sendNEXTIONcmd("xstr 0,18,390,18,1,RED,WHITE,0,1,1,\"SYSTEM> ... bStart push\"");
    }

    if (ev==bGame1){
      handlePageChange(2);
    }

    if (ev==bGame2){
      handlePageChange(3);
    }

    if (currentPage == "pageSimon") {
      if (ev == bGame1Start || ev == bGame1Retry || ev == bGame1Start2) {  // startSimonGame
        ev = "";
        Serial.print("Simon started");
        simon.playFullGame();
      } 
    }
    Serial.println("currentPage 1: "+ currentPage);

    if (currentPage == "pageDodge") {
      if (ev == bGame2Start) { 
        Serial.println("Juego Dodge iniciado");
        dodge.startGame();
      } else if (ev == bGame2End) {
        Serial.println("Juego Dodge finalizado");
        dodge.stopGame();
      }
    }

  }

  if (currentPage == "pageSimon") {
    simon.update();
  }
  
  if (currentPage == "pageDodge") {
    dodge.update();
  }
  /*
  if (digitalRead(buttonJoy) == HIGH) {
    sendNEXTIONcmd("game1=1");
    sendNEXTIONcmd("game2=1");
    sendNEXTIONcmd("game3=1");
    sendNEXTIONcmd("game4=1");
  }
  */
  delay(100);
}

// Manejo de cambio de página
void handlePageChange(int pageId) {
  String pageCommand;
  String prevPage = currentPage;
  
  switch(pageId) {
    case 0: 
      currentPage = "pageHome";
      pageCommand = "page 0";
      break;
    case 2:
      currentPage = "pageSimon";
      pageCommand = "page 2";
      break;
    case 3:
      currentPage = "pageDodge";
      pageCommand = "page 3";
      break;
    default: 
      currentPage = "unknown";
      pageCommand = "page 0";  // Por defecto volvemos a página 0
      break;
    
    if (prevPage == "pageSimon"){
      simon.stopGame();
    }
  }
  
  // Confirmar el cambio de página a Nextion
  sendNEXTIONcmd(pageCommand.c_str());
  
  Serial.print("Cambiando a página: ");
  Serial.println(currentPage);
  
  // Esperar un poco para que se complete el cambio
  delay(100);
}