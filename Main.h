#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "Wire.h"

#include "Debug.h"
#include "Global.h"
#include "MainFunction.h"
#include "BeamerFunction.h"
#include "IntensityController.h"
#include "WindowController.h"
#include "WeatherController.h"

class Main
{
    // Felder
private:
    Delegate<bool>* MasterAudio;      // Prüft ob das Audio eingeschaltet ist
    Delegate<bool>* MasterVideo;      // Prüft ob das Video eingeschaltet ist
    Delegate<bool>* MasterNutzungF1;  // Prüft ob die Nutzung von Forum 1 eingeschaltet ist
    Delegate<bool>* MasterNutzungF2;  // Prüft ob die Nutzung von Forum 1 eingeschaltet ist
  
public:
    MainFunction Audio;               // Hauptfunktion Audio
    MainFunction Video;               // Hauptfunktion Video
    MainFunction Licht;               // Hauptfunktion Light
    MainFunction NutzungF1;           // Hauptfunktion Nutzung F1
    MainFunction NutzungF2;           // Hauptfunktion Nutzung F2
    MainFunction DMK;                 // Hauptfunktion Dolmetscherkabinen
    MainFunction Livestream;          // Hauptfunktion Livestream
    MainFunction SeWo;                // Hauptfunktion SeWo
    MainFunction Pflegeheim;          // Hauptfunktion Pflegeheim
    BeamerFunction Beamer;            // Hauptfunktion Beamer
    WindowController WinControl;      // Fenster- und Jalousiesteuerung
    IntensityController IntControl;   // Helligkeitssteuerung
    WeatherController WeatherControl; // Wetter und Foyer Licht

    // Konstruktor
public:
    Main();

    // Methoden
public:
    // Initialisiert das Programm
    void Begin();

    // Muss in jedem Frame aufgerufen werden
    void Update();
};

#endif
