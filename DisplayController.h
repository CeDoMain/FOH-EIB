#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include "Arduino.h"
#include "LiquidCrystal.h"

#include "Timer.h"
#include "NkkKey.h"
#include "LinkedList.h"
#include "Config/Texts.h"
#include "Config/Values.h"
#include "Config/Connections.h"

class DisplayController
{
  // Aufzählungen
public:
  struct Error
  {
    bool Occurred;
    const __FlashStringHelper* Name;
    byte ID;
  };

  // Felder
private:
  LiquidCrystal Lcd;            // Anzeigehardware
  NkkKey* ErrorKey;             // Taster zum Weiterschalten und Quittieren der Fehler
  Timer MessageTimeout;         // Timer, der Meldungen ausblendet
  Timer ErrorTimeout;           // Timer, der Fehler ausblendet
  LinkedList<Error*> Errors;     // Liste mit registrierten Fehlern
  byte RegisteredErrorCount;    // Anzahl der registrierten Fehler
  byte ShowedError;             // ID des anzuzeigenden Fehlers
  bool* IsKeyActive;            // Gibt an, ob der Schlüsselschalter aktiv ist

  // Konstruktor
public:
  DisplayController(NkkKey* errorKey, bool* isKeyActive);

  // Methoden
public:
  // Initialisiert den DisplayController
  void Begin();

  // Muss in jedem Frame aufgerufen werden
  void Update();

  // Zeigt eine Meldung an
  void ShowMessage(const __FlashStringHelper* msg1, const __FlashStringHelper* msg2 = 0, unsigned int intervall = TIME_DISP_MSGTIMEOUT);
  void ShowMessage(char* msg1, char* msg2 = 0, unsigned int intervall = TIME_DISP_MSGTIMEOUT);
  void ShowIntensity(byte value, byte max);

  // Registriert einen Fehler und gibt seine ID zurück
  byte RegisterError(const __FlashStringHelper* name);

  // Löst eine Fehlermeldung aus
  void ErrorOccured(byte id);

  // Ändert die Helligkeit der Hintergrundbeleuchtung [0;1]
  void SetIntensity(float intensity);

  // Ändert den Kontrast des Display [0;1]
  void SetContrast(float contrast);

private:
  // Zeigt den Fehler mit der angegebenen ID an
  void ShowError();

  // Startet den ClearScreen-Timeout neu
  void RestartErrorTimeout();

  // Löscht das Display
  void ClearScreen();

  // Zeigt die nächste Fehlermeldung an
  void NextError();

  // Quittiert eine Fehlermeldung
  void AcknowledgeError();

  // Zeigt zwei Zeilen Text an
  void PrintTwoLine(const __FlashStringHelper* msg1, const __FlashStringHelper* msg2 = 0);
};

#endif
