#ifndef INTENSITYCONTROLLER_H
#define INTENSITYCONTROLLER_H

#include "Global.h"
#include "Fader.h"
#include "Config/Values.h"

class IntensityController
{
  // Felder
private:
  NkkKey* IntensityKey;      // Taster, der Helligkeit und Kontrast steuert
  Timer DimmerTimeout;          // Timer, der Taster und Anzeige dimmt
  Fader Fade;                   // Fader, der Taster und Anzeige dimmt
  float ActiveIntensity;        // Helligkeit nach Tastendruck
  const float DimmedIntensity;  // Helligkeit im Standby
  const float DimmTime;         // Zeit in s, in der gedimmt wird
  const float WakeUpTime;       // Zeit in s, in der die Aktiv-Helligkeit eingefadet wird

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
  void FadeChanged(float value);

  // Dimmt das Display und die Taster
  void DimmIntensity();

  // Stell Display und Tastatur wieder auf die Aktiv-Helligkeit
  void WakeUp();

  // Verändert die Aktiv-Helligkeit oder den Displaykontrast (wenn Schlüssel aktiv)
  void SwitchIntensity();
};

#endif
