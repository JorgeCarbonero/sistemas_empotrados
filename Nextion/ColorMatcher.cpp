#include "ColorMatcher.h"
#include "NextionObject.h"

ColorMatcher::ColorMatcher(int potPin, int btnR, int btnG, int btnB)
 : _potPin(potPin), _buttonRed(btnR), _buttonGreen(btnG), _buttonBlue(btnB) {
    reset();
}

void ColorMatcher::setPlaying(bool p) {
  _isPlaying = p;
}

void ColorMatcher::init() {
    reset();
    randomSeed(analogRead(A1));
}

void ColorMatcher::handleEvent(const String& ev) {
    if (ev == bColorStart) {
        if (!_isPlaying) {
          startGame();
        }
        else            stopGame();
    }
    else if (ev == bColorSubmit) {
        submitGame();
    }
}

void ColorMatcher::update() {
    if (!_isPlaying) return;
    handleButtons();
    handlePot();
    updateNextionDisplay();
}

void ColorMatcher::startGame() {
    // 1) Generar color objetivo
    for (int i = 0; i < 3; i++) {
        _targetColor[i] = random(0, 256);
    }
    int rndColor = (_targetColor[0] << 16) | (_targetColor[1] << 8) | _targetColor[2];

    // 2) Pintar b0 con color aleatorio
    sendNEXTIONcmd((String("b0.bco=") + rndColor).c_str());
    Serial.println((String("b0.bco=") + rndColor).c_str());
    sendNEXTIONcmd("ref b0");

    // 3) Pintar b1 en blanco
    sendNEXTIONcmd("b1.bco=16777215"); // 0xFFFFFF
    sendNEXTIONcmd("ref b1");

    // 4) Reiniciar valores y activar juego
    reset();
    _isPlaying = true;
}

void ColorMatcher::stopGame() {
    _isPlaying = false;
    sendNEXTIONcmd("pageColor.fail.en=0");
}

void ColorMatcher::submitGame() {
    bool success = true;
    for (int i = 0; i < 3; ++i) {
        if (abs(_colorValues[i] - _targetColor[i]) > 10) {
            success = false;
            break;
        }
    }
    if (success) {
        sendNEXTIONcmd("game4=1");
    } else {
        sendNEXTIONcmd("pageColor.fail.en=1");
    }
    _isPlaying = false;
}

void ColorMatcher::reset() {
    for (int i = 0; i < 3; ++i) _colorValues[i] = 0;
    _selectedColor = 0;
    _isPlaying = false;
}

void ColorMatcher::handleButtons() {
    if (digitalRead(_buttonRed)   == HIGH) { _selectedColor = 0; delay(200); }
    if (digitalRead(_buttonGreen) == HIGH) { _selectedColor = 1; delay(200); }
    if (digitalRead(_buttonBlue)  == HIGH) { _selectedColor = 2; delay(200); }
}

void ColorMatcher::handlePot() {
    int pot = analogRead(_potPin);
    _colorValues[_selectedColor] = map(pot, 0, 1023, 0, 255);
}

void ColorMatcher::updateNextionDisplay() {
    // 1) Mostrar valores en texto
    char buf[40];
    snprintf(buf, sizeof(buf), "t1.txt=\"Red:%d\"",   _colorValues[0]); sendNEXTIONcmd(buf);
    snprintf(buf, sizeof(buf), "t2.txt=\"Green:%d\"", _colorValues[1]); sendNEXTIONcmd(buf);
    snprintf(buf, sizeof(buf), "t3.txt=\"Blue:%d\"",  _colorValues[2]); sendNEXTIONcmd(buf);
    sendNEXTIONcmd("ref t1"); sendNEXTIONcmd("ref t2"); sendNEXTIONcmd("ref t3");

    // 2) Pintar b1 con el color actual
    int c = (_colorValues[0] << 16) | (_colorValues[1] << 8) | _colorValues[2];
    sendNEXTIONcmd((String("b1.bco=") + c).c_str());
    sendNEXTIONcmd("ref b1");
}
