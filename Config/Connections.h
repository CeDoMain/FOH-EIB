#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include "ArduinoMega.h"

// I2C Adressen der Tasterplatinen
#define CONN_I2C_CONTROL      B000
#define CONN_I2C_MAINFUNC_1   B001
#define CONN_I2C_MAINFUNC_2   B010
#define CONN_I2C_WINDOW_1     B110          // A0 ist defekt => wird immer als GND erkannt
#define CONN_I2C_WINDOW_2     B100

// Anschluss für den Schlüsselschalter (Schließer)
#define CONN_KEY              ArduinoMega::DigitalPin::D7

// Verbindungen zum Display
#define CONN_DISP_VEE         3
#define CONN_DISP_RS          52
#define CONN_DISP_RW          50
#define CONN_DISP_E           48
#define CONN_DISP_D0          46
#define CONN_DISP_D1          44
#define CONN_DISP_D2          42
#define CONN_DISP_D3          40
#define CONN_DISP_D4          38
#define CONN_DISP_D5          36
#define CONN_DISP_D6          34
#define CONN_DISP_D7          32
#define CONN_DISP_LED         2

// KNX Objekte für Beamersteuerung
#define KNX_BEAMER_SWITCH             30    // Ein (1) und Ausschalten (0) des Beamers
#define KNX_BEAMER_GETSTATE           31    // Statusabfrage (1)
#define KNX_BEAMER_OFF                33    // Betriebsbereit (1)
#define KNX_BEAMER_WARMUP             34    // Zurückzählen beim Ausschalten (1)
#define KNX_BEAMER_ON                 35    // Aufwärmen beim Anschalten (1)
#define KNX_BEAMER_COOLDOWN           36    // Abkühlen nach Zurückzählen (1)

#define KNX_WEATHER_DATETIMEREQUEST   0     // Datumsabfrage (1)
#define KNX_WEATHER_WEATHERREQUEST    1     // Temperaturminimum- und -maximumabfrage (1)
#define KNX_WEATHER_DATE              2     // aktuelles Datum (auf Anfrage)
#define KNX_WEATHER_TIME              3     // aktuelle Uhrzeit (auf Anfrage)
#define KNX_WEATHER_TEMP              4     // aktuelle Temperatur (zyklisch und bei Änderung)
#define KNX_WEATHER_TEMPMIN           5     // minimale gemessene Temperatur (auf Anfrage)
#define KNX_WEATHER_TEMPMAX           6     // maximale gemessene Temperatur (auf Anfrage)
#define KNX_WEATHER_WIND              7     // aktuelle Windgeschwindigkeit (zyklisch und bei Änderung)
#define KNX_WEATHER_WINDALARM         8     // Windalarm für Jalousien (1) bzw. (0) bei Freigabe
#define KNX_WEATHER_RAINALARM         9     // Regenalarm für Fenster (1) bzw. (0) bei Freigabe
#define KNX_WEATHER_FOYERLIGHTON      69    // Licht im Foyer einschalten (1)

#define KNX_GLOBAL_LIGHTFOH           68    // Schaltet das Licht unter den FOH Tischen

#endif
