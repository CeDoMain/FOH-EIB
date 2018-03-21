#ifndef GLOBAL_H
#define GLOBAL_H

#include "MCP23017.h"
#include "ArduinoMega.h"
#include "Button.h"
#include "BiColorLED.h"
#include "SIMKNX128.h"
#include "ArduinoMega.h"
#include "DisplayController.h"
#include "NkkKey.h"
#include "Debug.h"
#include "MemoryFree.h"
#include "Config/Connections.h"
#include "Config/Keys.h"

class Global
{
    // Felder
public:
    static PCA9685 PCMchip[5];            // LED PCM Controller
    static MCP23017 GPIOchip[5];          // Taster GPIO Controller
    static NkkKey Key[5][5];              // Speichert Steuerung und Pins für jeden Button
    static bool IsKeyLockInactive;        // Gibt an, ob die Schlüsselfreigabe aktiv ist
    static DisplayController Disp;        // Displayverwaltung
    static SIMKNX128 Knx;                 // KNX-EIB Schnittstelle
    static unsigned int LoopFps;          // Wiederholrate der Loop
private:
    static Button KeyLockBtn;             // Schließkontakt des Schlüsselschalters

    // Konstruktor
private:
    Global() {}

    // Methoden
public:
    // Initialisiert die Hardware
    static void Begin();

    // Muss in jedem Frame aufgerufen werden
    static void Update();

private:
    // Wird aufgerufen, wenn der Schlüssel 2 s lang gehalten wurde
    static void KeyLockActivate();

    // Wird aufgerufen, wenn der Schlüssel losgelassen wurde
    static void KeyLockDeactivate();
};

#endif
