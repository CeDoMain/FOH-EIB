#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include "Arduino.h"
#include "Timer.h"
#include "LinkedList.h"
#include "Global.h"

class WindowController
{
  // Datentypen
public:
    struct KnxObjectData
    {
      const byte MoveKnxObjectIndex;        // SIMKNX Objekt für Fahren
      const byte StepKnxObjectIndex;        // SIMKNX Objekt für Schritt
      const byte UpTriggerKnxObjectIndex;   // SIMKNX Objekt für Rückmeldung oben
      const byte DownTriggerKnxObjectIndex; // SIMKNX Objekt für Rückmeldung unten
      const __FlashStringHelper* Name;      // Name des Fenster oder der Jalousie
      NkkKey* Key;                          // Auswahltaster
      Delegate<bool>* CanActivate;          // Gibt an, ob Steuerung erlaubt ist
      const bool IsColorReversed;           // Gibt an, ob Rot/Grün vertauscht ist
    };

  // Felder
private:
  NkkKey* UpKey;                            // Jalousie nach oben bzw. Fenster auf
  NkkKey* DownKey;                          // Jalousie nach unten bzw. Fenster zu
  LinkedList<KnxObjectData*>* Windows;      // Speichert die Fenster und Jalousien
  KnxObjectData* SelectedWindow;            // Zeiger auf das ausgewählte Fenster oder Jalousie
  Timer DeselectTimer;                      // Löscht die Auswahl nach einer bestimmten Zeit
  const float SelectionPulseRate;           // Geschwindigkeit mit der die Auswahltasten bei einer Auswahl blinken

  // Konstruktor
public:
  WindowController(NkkKey* upKey, NkkKey* downKey);

  // Methoden
public:
  // Initialisiert den WindowController
  void Begin();

  // Muss in jedem Frame aufgerufen werden
  void Update();

  // Fügt ein Fenster hinzu
  void AddWindow(
    const byte MoveKnxObjectIndex,
    const byte StepKnxObjectIndex,
    const byte UpTriggerKnxObjectIndex,
    const byte DownTriggerKnxObjectIndex,
    const __FlashStringHelper* Name,
    NkkKey* Key,
    Delegate<bool>* CanActivate);

  // Fügt eine Jalousie hinzu
  void AddJalousie(
    const byte MoveKnxObjectIndex,
    const byte StepKnxObjectIndex,
    const byte UpTriggerKnxObjectIndex,
    const byte DownTriggerKnxObjectIndex,
    const __FlashStringHelper* Name,
    NkkKey* Key,
    Delegate<bool>* CanActivate);

private:
  // Wird aufgerufen, wenn eine Auswahl gedrückt wurde
  void Select(KnxObjectData* obj);

  // Wird aufgerufen, wenn die Zeit abgelaufen ist oder bevor eine andere Auswahl getroffen wird
  void Deselect();

  // Wird aufgerufen, wenn der Hoch-Taster kurz gedrückt wird
  void StepUp();

  // Wird aufgerufen, wenn der Runter-Taster kurz gedrückt wird
  void StepDown();

  // Wird aufgerufen, wenn der Hoch-Taster lang gedrückt wird
  void MoveUp();

  // Wird aufgerufen, wenn der Runter-Taster lang gedrückt wird
  void MoveDown();

};

#endif
