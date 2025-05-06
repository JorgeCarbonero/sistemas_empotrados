#include "NextionSoftSerial.h"
#include "NextionObject.h"
#include "SimonGame.h"
#include "DodgeGame.h"
#include "ColorMatcher.h"

// Pines físicos
const int potPin = A0;
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

// SimonGame
const int simonEntradas[4] = {buttonRed, buttonGreen, buttonBlue, buttonYellow};
const int simonSalidas [4] = {redPin,   greenPin,   bluePin,   yellowPin};
SimonGame simon(simonEntradas, simonSalidas);
// DodgeGame
DodgeGame dodge(joyX, joyY);
ColorMatcher colorMatcher(potPin, buttonRed, buttonGreen, buttonBlue);

// Estado de ColorMatcher OBJETO
bool playingColorMatcher = false;
int selectedColor = 0;
int colorValues[3] = {0, 0, 0};
int targetColor[3] = {0, 0, 0};

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
  sendNEXTIONcmd("xstr 0,18,390,18,1,RED,WHITE,0,1,1,\"SYSTEM> Initializing ... Nextion_SoftSerial\"");
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

  randomSeed(analogRead(A1)); 
  colorMatcher.init();
}

void loop() {
  String ev = listenNEXTION();
  char cmd[100];

  if (ev != "") {
    Serial.print("Evento recibido: ");
    Serial.println(ev.c_str());
    //Serial.println(ev);
    
    if (ev==bStart) 
    {
      strcpy(cmd,"\r\nSYSTEM> bStart push");
      Serial.println(cmd);

      sendNEXTIONcmd("xstr 0,18,390,18,1,RED,WHITE,0,1,1,\"SYSTEM> ... bStart push\"");
    }

    if (ev==bGame1){
      handlePageChange(2);
    }

    if (ev==bGame2){
      handlePageChange(3);
    }

    // Manejo de cambio de página
    if (ev==bGame4) {
      colorMatcher.setPlaying(false);
      handlePageChange(5);
    }

    if (currentPage == "pageSimon") {
      if (ev == bGame1Start || ev == bGame1Retry) {  // startSimonGame
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
        dodge.stopGame();
      }
    }


    if (currentPage == "pageColor") {
      Serial.println("MAL");
      if (ev == bColorStart) {
        Serial.println("Start");
      }
      if (ev == bColorSubmit) {
        Serial.println("submit");
      }
    }

  }

  if (currentPage == "pageSimon") {
    simon.update();
  }
  
  if (currentPage == "pageDodge") {
    dodge.update();
  }

  if (currentPage == "pageColor") {
    colorMatcher.update();
  }
  
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
    case 5: 
      currentPage = "pageColor"; 
      pageCommand = "page pageColor";
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