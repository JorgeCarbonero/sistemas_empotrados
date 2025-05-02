#ifndef DODGE_GAME_H
#define DODGE_GAME_H

#include <Arduino.h>
#include "NextionSoftSerial.h"

class DodgeGame {
public:
    // Zonas muertas para no detectar pequeños ruidos
    static const int DEADZONE = 200;

    // Posibles direcciones
    enum Direction { NONE = 0, UP, DOWN, LEFT, RIGHT };

    // Constructor: pines analógicos X e Y del joystick
    DodgeGame(int pinX, int pinY);

    // Arranca el juego (empezar a leer y enviar)
    void startGame();

    // Para el juego (dejar de leer y enviar)
    void stopGame();

    // Si cambia la dirección, la envía a Nextion en t0
    void update();

private:
    int      _pinX, _pinY;
    Direction _lastDir;
    bool      _running;

    // Nombres para mostrar
    static const char* const _dirNames[5];

    // Lectura bruta de joystick -> Direction
    Direction readDirection();
};

#endif // DODGE_GAME_H
