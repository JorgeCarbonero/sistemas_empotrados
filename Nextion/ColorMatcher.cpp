class ColorMatcher {
private:
  int colorValues[3] = {0, 0, 0};
  int targetColor[3] = {0, 0, 0};
  int selectedColor = 0;
  bool isPlaying = false;
  bool isStarted = false;

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
    sendNEXTIONcmd("b0.bco=" + String(color24bit));
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
    sendNEXTIONcmd("t1.txt=\"Red:" + String(colorValues[0]) + "\"");
    sendNEXTIONcmd("t2.txt=\"Green:" + String(colorValues[1]) + "\"");
    sendNEXTIONcmd("t3.txt=\"Blue:" + String(colorValues[2]) + "\"");
    sendNEXTIONcmd("ref t1");
    sendNEXTIONcmd("ref t2");
    sendNEXTIONcmd("ref t3");
  }
};
