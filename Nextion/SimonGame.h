// SimonGame.h
#ifndef SIMON_GAME_H
#define SIMON_GAME_H

#include <Arduino.h>
#include "NextionSoftSerial.h"
#include "NextionObject.h"

class SimonGame {
public:
    static const int NIVEL_MAX = 6;

    static const char* const colorNames[4];
    // Ahora sólo recibimos entradas y salidas
    SimonGame(const int entradaPins[4],
              const int salidaPins[4]);

    void update();                   // Una ronda: genera/muestra/lee
    void playFullGame();             // Juega todos los niveles seguidos
    int  getLevel() const;           // Nivel actual
    void stopGame();

private:
    int _entrada[4];
    int _salida[4];

    int _nivelActual;
    int _velocidad;
    int _secuencia[NIVEL_MAX];
    int _secuenciaUsuario[NIVEL_MAX];
    bool _running;

    void generaSecuencia();
    void muestraSecuencia();
    bool leeSecuencia();
    void secuenciaError();
    void secuenciaCorrecta();
};

#endif
