#include "NextionSoftSerial.h"
#include "NextionObject.h"
#include "SimonGame.h"

// Pines físicos
const int potPin = A0;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int whitePin = 12
const int buttonRed = 4;
const int buttonGreen = 5;
const int buttonBlue = 6;
const int buttonWhite = 7;
const int joyX = A3
const int joyY = A4

// SimonGame
const int simonEntradas[4] = {buttonRed, buttonGreen, buttonBlue, buttonWhite};
const int simonSalidas [4] = {redPin,   greenPin,   bluePin,   whitePin};
SimonGame simon(simonEntradasm, simonsalidas)

// DodgeGame
DodgeGame dodge(joyX, joyY);

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
  pinMode(whitePin, OUTPUT)

  pinMode(buttonRed, INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonBlue, INPUT_PULLUP);
  pinMode(buttonWhite, INPUT_PULLUP)

  pinMode(_pinX, INPUT);
  pinMode(_pinY, INPUT);

  randomSeed(analogRead(A1)); 
  resetColorMatcher();
}

void loop() {
  String ev = listenNEXTION();
  
  if (ev != "") {
    Serial.print("Evento recibido: ");
    Serial.println(ev.c_str());
    Serial.println(ev);
    
    if (ev==bStart) 
    {
      strcpy(cmd,"\r\nSYSTEM> bStart push");
      Serial.println(cmd);

      sendNEXTIONcmd("xstr 0,18,390,18,1,RED,WHITE,0,1,1,\"SYSTEM> ... bStart push\"");
    }

    

    if (ev==bGame1){
      handlePageChange(2)
    }

    if (ev==bGame2){
      handlePageChange(3)
    }

    // Manejo de cambio de página
    if (ev==bGame4) {
      handlePageChange(5);
    }

    if (currentPage == "pageSimon") {
      if (ev == bGame1Start) {  // startSimonGame
        bool won = simon.playFullGame();
         if (won) {
          sendNEXTIONcmd("vaState.val=2");
        } else {
          sendNEXTIONcmd("vaState.val=1");
        }
      } 
    }
    
    if (currentPage == "pageSimon") {
      if (ev == bGame2Start) { 
        dodge.startGame();
      } else if (ev == bGame2End) {
        dodge.endGame();
      }
    }

    case ///// TAREK!!!
    // Comandos específicos de ColorMatcher
    if (currentPage == "pageColor") {
      if (ev == "[65001ffffffffffff]") {  // startColorMatcher
        
      } else if (ev == "[65002ffffffffffff]") {  // submitColorMatcher
        
      }
    }
  }
  
  if (currentPage == "pageDodge") {
    dodge.update();
  }

  if (playingColorMatcher && currentPage == "pageColor") {
    handleButtonsColorMatcher();
    handlePotColorMatcher();
    updateLEDColorMatcher();
    sendCurrentRGBColorMatcher();
  }
  
  delay(100);
}

// Manejo de cambio de página
void handlePageChange(int pageId) {
  String pageCommand;
  
  switch(pageId) {
    case 0: 
      currentPage = "pageHome";
      pageCommand = "page 0";
      break;
    case 2:
      currentPage = "PageSimon";
      pageCommand = "page 2";
      break;
    case 3:
      currentPage = "PageDodge";
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
  }
  
  // Confirmar el cambio de página a Nextion
  sendNEXTIONcmd(pageCommand.c_str());
  
  Serial.print("Cambiando a página: ");
  Serial.println(currentPage);
  
  // Esperar un poco para que se complete el cambio
  delay(100);
}

// Funciones ColorMatcher (sin cambios en la lógica)
void startColorMatcherGame() {
  targetColor[0] = random(0, 256);
  targetColor[1] = random(0, 256);
  targetColor[2] = random(0, 256);
  
  int color24bit = (targetColor[0] << 16) | (targetColor[1] << 8) | (targetColor[2]);
  sendNEXTIONcmd("b0.bco=" + String(color24bit)); #texto 
  sendNEXTIONcmd("ref b0");

  resetColorMatcher();
  playingColorMatcher = true;
}

void submitColorMatcherGame() {
  bool success = true;
  for (int i = 0; i < 3; i++) {
    if (abs(colorValues[i] - targetColor[i]) > 10) {
      success = false;
    }
  }
  if (success) {
    sendNEXTIONcmd("game4=1");
  } else {
    sendNEXTIONcmd("pageColor.fail.en=1"); #error
  }
  playingColorMatcher = false;
}

void resetColorMatcher() {
  colorValues[0] = 0;
  colorValues[1] = 0;
  colorValues[2] = 0;
}

void handleButtonsColorMatcher() {
  if (digitalRead(buttonRed) == HIGH) { selectedColor = 0; delay(200); }
  if (digitalRead(buttonGreen) == HIGH) { selectedColor = 1; delay(200); }
  if (digitalRead(buttonBlue) == HIGH) { selectedColor = 2; delay(200); }
}

void handlePotColorMatcher() {
  int potValue = analogRead(potPin);
  int mappedValue = map(potValue, 0, 1023, 0, 255);
  colorValues[selectedColor] = mappedValue;
}

void updateLEDColorMatcher() {
  analogWrite(redPin, colorValues[0]);
  analogWrite(greenPin, colorValues[1]);
  analogWrite(bluePin, colorValues[2]);
}

void sendCurrentRGBColorMatcher() {
  sendNEXTIONcmd("t1.txt=\"Red:" + String(colorValues[0]) + "\"");
  sendNEXTIONcmd("t2.txt=\"Green:" + String(colorValues[1]) + "\"");
  sendNEXTIONcmd("t3.txt=\"Blue:" + String(colorValues[2]) + "\"");
  sendNEXTIONcmd("ref t1");
  sendNEXTIONcmd("ref t2");
  sendNEXTIONcmd("ref t3");
}