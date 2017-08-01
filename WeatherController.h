#ifndef WEATHERCONTROLLER_H
#define WEATHERCONTROLLER_H

#include "Event.h"
#include "Timer.h"

#include "Global.h"

class WeatherController
{
  // Felder
private:
  NkkKey* WeatherKey;                       // Taster zum Steuern der Wetterabfrage und Beleuchtung Foyer
  const byte SendLightFoyer;                // Licht im Foyer an
  const byte SendDateTimeRequest;           // KNX Objekt um das Datum und die Uhrzeit abzufragen
  const byte SendWeatherRequest;            // KNX Objekt um die Minimal und Maximal Außentemperatur abzufragen
  const byte RecvDate;
  const byte RecvTime;
  const byte RecvTemperature;
  const byte RecvTemperatureMin;
  const byte RecvTemperatureMax;
  const byte RecvWind;
  byte Day, Month, Hour, Minute;
  word Year;
  float Temperature, TemperatureMin, TemperatureMax, Wind;
  Timer DisplayDateTime, DisplayWeather;

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
};

#endif
