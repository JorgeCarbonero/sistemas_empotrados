// SimonGame.cpp
#include "SimonGame.h"

// Definición de los nombres
const char* const SimonGame::colorNames[4] = {
    "Rojo", "Verde", "Azul", "Blanco"
};

SimonGame::SimonGame(const int entradaPins[4],
                     const int salidaPins[4])
    : _nivelActual(4),
      _velocidad(500)
{
    for (int i = 0; i < 4; i++) {
        _entrada[i] = entradaPins[i];
        Serial.println(_entrada[i]);
        _salida[i]  = salidaPins[i];
    }
}

bool SimonGame::update() {
    if (_nivelActual == 4) generaSecuencia();
    muestraSecuencia();
    // Renombrada la variable para no chocarnos con la palabra reservada
    bool continueGame = leeSecuencia();
    return continueGame;
}

bool SimonGame::playFullGame() {
    _nivelActual = 4;
    _velocidad   = 500;
    bool continueGame;

    do {
        continueGame = update();
        if (_nivelActual == 1) return false;  // fallaste en el nivel 4
    } while (continueGame && (_nivelActual > 4 && _nivelActual < NIVEL_MAX));

    if (_nivelActual == NIVEL_MAX) {
        sendNEXTIONcmd("vaState.val=2");
        Serial.print("Simon game won");
    }

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
        String cmd = String("t0.txt=\"Mostrando: ");
        cmd += colorNames[idx];
        cmd += "\"";
        sendNEXTIONcmd(cmd.c_str());
        sendNEXTIONcmd("ref t0");

        // Mostrar en Arduino
        Serial.print("Mostrando: ");
        Serial.println(colorNames[idx]);

        // Encender LED
        digitalWrite(_salida[idx], HIGH);
        delay(_velocidad);
        digitalWrite(_salida[idx], LOW);
        delay(200);
    }
}

bool SimonGame::leeSecuencia() {
    for (int i = 0; i < _nivelActual; i++) {
        bool leido = false;
        while (!leido) {
            for (int btn = 0; btn < 4; btn++) {
                if (digitalRead(_entrada[btn]) == HIGH) {
                    // Mostrar en Nextion
                    String cmd = String("t0.txt=\"Has pulsado: ");
                    cmd += colorNames[btn];
                    cmd += "\"";
                    sendNEXTIONcmd(cmd.c_str());
                    sendNEXTIONcmd("ref t0");

                    // Mostrar en Arduino
                    Serial.print("Has pulsado: ");
                    Serial.println(colorNames[btn]);

                    // Feedback visual
                    digitalWrite(_salida[btn], HIGH);
                    delay(300);
                    digitalWrite(_salida[btn], LOW);
                    delay(200);

                    _secuenciaUsuario[i] = btn;
                    leido = true;
                    if (_secuenciaUsuario[i] != _secuencia[i]) {
                        Serial.print("hola");
                        secuenciaError();
                        return false;
                    }
                }
            }
        }
    }
    secuenciaCorrecta();
    return true;
}

void SimonGame::secuenciaError() {
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < 4; i++) digitalWrite(_salida[i], HIGH);
        delay(250);
        for (int i = 0; i < 4; i++) digitalWrite(_salida[i], LOW);
        delay(250);
    }
    sendNEXTIONcmd("vaState.val=1");
    Serial.println("Simon game lost");
}

void SimonGame::secuenciaCorrecta() {
    if (_nivelActual < NIVEL_MAX) _nivelActual++;
    _velocidad = max(100, _velocidad - 50);
    delay(200);
}
