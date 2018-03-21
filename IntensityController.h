#ifndef INTENSITYCONTROLLER_H
#define INTENSITYCONTROLLER_H

#include "Global.h"
#include "Fader.h"
#include "Config/Values.h"

class IntensityController
{
    // Felder
  private:
    NkkKey* IntensityKey;               // Taster, der Helligkeit und Kontrast steuert
    Timer DimmerTimeout;                // Timer, der Taster und Anzeige dimmt
    Fader Fade;                         // Fader, der Taster und Anzeige dimmt
    decimal ActiveIntensity;            // Helligkeit nach Tastendruck

    // Konstruktor
public:
    IntensityController(NkkKey* intensityKey);

    // Methoden
public:
    // Initialisiert den IntensityController
    void Begin();

    // Muss in jedem Frame aufgerufen werden
    void Update();

private:
    // Setzt die Helligkeit als neuen globalen Wert
    void FadeChanged(decimal value);

    // Dimmt das Display und die Taster
    void DimmIntensity();

    // Stell Display und Tastatur wieder auf die Aktiv-Helligkeit
    void WakeUp();

    // Verändert die Aktiv-Helligkeit oder den Displaykontrast (wenn Schlüssel aktiv)
    void SwitchIntensity();
};

#endif
