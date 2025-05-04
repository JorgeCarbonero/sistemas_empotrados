#include "DodgeGame.h"

// Textos para cada dirección
const char* const DodgeGame::_dirNames[5] = {
    "Ninguna", "Arriba", "Abajo", "Izquierda", "Derecha"
};

DodgeGame::DodgeGame(int pinX, int pinY)
  : _pinX(pinX), _pinY(pinY), _lastDir(NONE), _running(false)
{}

void DodgeGame::startGame() {
    Serial.print("juego iniciado");
    _lastDir = NONE;
    _running = true;
}

void DodgeGame::stopGame() {
    _running = false;
}

DodgeGame::Direction DodgeGame::readDirection() {
    int x = analogRead(_pinX);
    int y = analogRead(_pinY);
    int center = 512;

    int dx = x - center;
    int dy = y - center;

    if (abs(dx) < DEADZONE && abs(dy) < DEADZONE) {
        return NONE;
    }
    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? RIGHT : LEFT;
    } else {
        return (dy > 0) ? UP : DOWN;
    }
}

void DodgeGame::update() {
    if (!_running) return;
    Direction dir = readDirection();
    if (dir != _lastDir) {
        _lastDir = dir;
    
        Serial.print("Joystick apunta a: ");
        Serial.println(_dirNames[dir]);

        // Construir el comando Nextion y enviar como const char*
        String cmd = String("vaUserDir.val=");
        cmd += dir;
        sendNEXTIONcmd(cmd.c_str());
    }
}
