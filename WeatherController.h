#ifndef WEATHERCONTROLLER_H
#define WEATHERCONTROLLER_H

#include "Event.h"
#include "Timer.h"

#include "Global.h"

// Festkommatyp mit 4 Nachkommastellen und Vorzeichen
typedef short decimal;

class WeatherController
{
  // Felder
private:
  NkkKey* WeatherKey;                       // Taster zum Steuern der Wetterabfrage und Beleuchtung Foyer
  byte Day, Month, Hour, Minute;
  word Year;
  decimal Temperature, TemperatureMin, TemperatureMax, Wind;
  Timer DisplayDateTime, DisplayWeather, DisplayAlarms;
  bool RainAlarm, WindAlarm;

  // Konstruktor
public:
  WeatherController(NkkKey* weatherKey);

  // Methoden
public:
  // Initialisiert den Wettercontroller
  void Begin();

  // Muss in jedem Frame aufgerufen werden
  void Update();

  // Wird aufgerufen, wenn eine Rückmeldung empfangen wurde
  void WeatherKnxObjectReceived(byte object, char* value);

  // Wird aufgerufen, wenn die Wetterstation abgefragt werden soll
  void RequestWeather();

  // Wird aufgerufen, wenn das Licht im Foyer eingeschaltet werden soll
  void SwitchFoyerLightOn();

  // Zeigt Datum und Uhrzeit auf dem Display an
  void ShowDateTime();

  // Zeigt Temperatur und Wind auf dem Display an
  void ShowWeather();

  // Zeigt Wind- und Regenalarm auf dem Display an
  void ShowAlarms();
};

#endif
