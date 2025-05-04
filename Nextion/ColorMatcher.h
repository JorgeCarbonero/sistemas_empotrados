// -------------------------
//  Archivo: ColorMatcher.h
// -------------------------
#ifndef _COLORMATCHER_H_
#define _COLORMATCHER_H_

#include <Arduino.h>
#include "NextionSoftSerial.h"

#define bColorStart   ("[65001ffffffffffff]")  // Start/Stop en Nextion
#define bColorSubmit  ("[65002ffffffffffff]")  // Submit en Nextion

class ColorMatcher {
public:
    ColorMatcher(int potPin = A0, int btnR = 4, int btnG = 5, int btnB = 6);
    void init();
    void handleEvent(const String& ev);
    void update();
    void setPlaying(bool p);

private:
    bool _isPlaying;
    int  _potPin, _buttonRed, _buttonGreen, _buttonBlue;
    int  _colorValues[3];
    int  _targetColor[3];
    int  _selectedColor;

    void startGame();
    void stopGame();
    void submitGame();
    void reset();
    void handleButtons();
    void handlePot();
    void updateNextionDisplay();
};

#endif // _COLORMATCHER_H_
