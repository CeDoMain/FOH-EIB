#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "Wire.h"

#include "MemoryFree.h"
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
  Delegate<bool> MasterAudio;
  Delegate<bool> MasterVideo;
  Delegate<bool> MasterNutzungF1;
  Delegate<bool> MasterNutzungF2;
  Delegate<bool> MasterNutzungF1AndRain;
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
