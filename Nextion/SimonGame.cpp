#include "SimonGame.h"

// Definición de los nombres
const char* const SimonGame::colorNames[4] = {
    "Rojo", "Verde", "Azul", "Blanco"
};

SimonGame::SimonGame(const int entradaPins[4],
                     const int salidaPins[4])
    : _nivelActual(1),
      _velocidad(500)
{
    for (int i = 0; i < 4; i++) {
        _entrada[i] = entradaPins[i];
        _salida[i]  = salidaPins[i];
    }
}

void SimonGame::begin() {
    for (int i = 0; i < 4; i++) {
        pinMode(_entrada[i], INPUT_PULLUP);
        pinMode(_salida[i], OUTPUT);
        digitalWrite(_salida[i], LOW);
    }
}

void SimonGame::update() {
    if (_nivelActual == 1) generaSecuencia();
    muestraSecuencia();
    leeSecuencia();
}

bool SimonGame::playFullGame() {
    _nivelActual = 1;
    _velocidad   = 500;

    do {
        update();
        if (_nivelActual == 1) return false;
    } while (_nivelActual > 1 && _nivelActual < NIVEL_MAX);

    return true;
}

int SimonGame::getLevel() const {
    return _nivelActual;
}

void SimonGame::generaSecuencia() {
    randomSeed(millis());
    for (int i = 0; i < NIVEL_MAX; i++) {
        _secuencia[i] = random(0, 4);
    }
}

void SimonGame::muestraSecuencia() {
    for (int i = 0; i < 4; i++) digitalWrite(_salida[i], LOW);

    for (int i = 0; i < _nivelActual; i++) {
        int idx = _secuencia[i];

        // Mostrar en Nextion
        sendNEXTIONcmd("t0.txt=\"Mostrando: " + String(colorNames[idx]) + "\"");
        sendNEXTIONcmd("ref t0");

        // Mostrar en arduino
        Serial.print("Mostrando: ");
        Serial.println(String(colorNames[idx]));

        // Encender LED
        digitalWrite(_salida[idx], HIGH);
        delay(_velocidad);
        digitalWrite(_salida[idx], LOW);
        delay(200);
    }
}

void SimonGame::leeSecuencia() {
    for (int i = 0; i < _nivelActual; i++) {
        bool leido = false;
        while (!leido) {
            for (int btn = 0; btn < 4; btn++) {
                if (digitalRead(_entrada[btn]) == LOW) {
                    // Mostrar en Nextion
                    sendNEXTIONcmd("t0.txt=\"Has pulsado: " + String(colorNames[btn]) + "\"");
                    sendNEXTIONcmd("ref t0");

                    // Mostrar en arduino
                    Serial.print("Has pulsado: ");
                    Serial.println(String(colorNames[btn]));

                    // Feedback visual
                    digitalWrite(_salida[btn], HIGH);
                    delay(300);
                    digitalWrite(_salida[btn], LOW);
                    delay(200);

                    _secuenciaUsuario[i] = btn;
                    leido = true;
                    if (_secuenciaUsuario[i] != _secuencia[i]) {
                        secuenciaError();
                        return;
                    }
                }
            }
        }
    }
    secuenciaCorrecta();
}

void SimonGame::secuenciaError() {
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < 4; i++) digitalWrite(_salida[i], HIGH);
        delay(250);
        for (int i = 0; i < 4; i++) digitalWrite(_salida[i], LOW);
        delay(250);
    }
    _nivelActual = 1;
    _velocidad   = 500;
}

void SimonGame::secuenciaCorrecta() {
    if (_nivelActual < NIVEL_MAX) _nivelActual++;
    _velocidad = max(100, _velocidad - 50);
    delay(200);
}
