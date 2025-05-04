#include <Arduino.h>
#include "NextionSoftSerial.h"

class ColorMatcher {
private:
  int colorValues[3] = {0, 0, 0};
  int targetColor[3] = {0, 0, 0};
  int selectedColor = 0;
  bool isPlaying = false;
  bool isStarted = false;
  const int redPin = 9;
  const int bluePin = 11;
  const int greenPin = 10;
  const int potPin = A0;
  const int buttonRed = 4;
  const int buttonGreen = 5;
  const int buttonBlue = 6;

public:
  void init() {
    reset();
    randomSeed(analogRead(A1));
  }

  void handleEvent(String ev) {
    if (ev == "[65001ffffffffffff]") {  // toggle start/stop
      if (!isStarted) {
        startGame();
        isStarted = true;
      } else {
        stopGame();
        isStarted = false;
      }
    } else if (ev == "[65002ffffffffffff]") {
      submitGame();
    }
  }

  void update() {
    if (!isPlaying) return;
    handleButtons();
    handlePot();
    updateLEDs();
    updateNextionDisplay();
  }

  void startGame() {
    for (int i = 0; i < 3; i++) {
      targetColor[i] = random(0, 256);
    }

    int color24bit = (targetColor[0] << 16) | (targetColor[1] << 8) | targetColor[2];
    String cmd = String("b0.bco=");
    cmd += color24bit;
    sendNEXTIONcmd(cmd.c_str());
    sendNEXTIONcmd("ref b0");

    reset();
    isPlaying = true;
  }

  void stopGame() {
    isPlaying = false;
    sendNEXTIONcmd("pageColor.fail.en=0");
  }

  void submitGame() {
    bool success = true;
    for (int i = 0; i < 3; i++) {
      if (abs(colorValues[i] - targetColor[i]) > 10) {
        success = false;
      }
    }
    if (success) {
      sendNEXTIONcmd("game4=1");
    } else {
      sendNEXTIONcmd("pageColor.fail.en=1");
    }
    isPlaying = false;
    isStarted = false;
  }

  void reset() {
    colorValues[0] = 0;
    colorValues[1] = 0;
    colorValues[2] = 0;
  }

  void handleButtons() {
    if (digitalRead(buttonRed) == HIGH) { selectedColor = 0; delay(200); }
    if (digitalRead(buttonGreen) == HIGH) { selectedColor = 1; delay(200); }
    if (digitalRead(buttonBlue) == HIGH) { selectedColor = 2; delay(200); }
  }

  void handlePot() {
    int potValue = analogRead(potPin);
    int mappedValue = map(potValue, 0, 1023, 0, 255);
    colorValues[selectedColor] = mappedValue;
  }

  void updateLEDs() {
    analogWrite(redPin, colorValues[0]);
    analogWrite(greenPin, colorValues[1]);
    analogWrite(bluePin, colorValues[2]);
  }

  void updateNextionDisplay() {
    char buf[32];

    // Red
    snprintf(buf, sizeof(buf), "t1.txt=\"Red:%d\"",   colorValues[0]);
    sendNEXTIONcmd(buf);

    // Green
    snprintf(buf, sizeof(buf), "t2.txt=\"Green:%d\"", colorValues[1]);
    sendNEXTIONcmd(buf);

    // Blue
    snprintf(buf, sizeof(buf), "t3.txt=\"Blue:%d\"",  colorValues[2]);
    sendNEXTIONcmd(buf);

    sendNEXTIONcmd("ref t1");
    sendNEXTIONcmd("ref t2");
    sendNEXTIONcmd("ref t3");
  }
};
