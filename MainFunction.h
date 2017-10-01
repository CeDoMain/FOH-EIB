#ifndef MAINFUNCTION_H
#define MAINFUNCTION_H

#include "Debug.h"
#include "LinkedList.h"

#include "Global.h"

class MainFunction
{
  // Datentypen
public:
  struct KnxObjectData
  {
    const byte FunctionKnxObjectIndex;
    bool Value;
    bool IsCorrectValue;
    const __FlashStringHelper* Name;
    const byte ErrorID;
  };

  // Felder
public:
  Event<> SwitchingOffEvent;                // Wird ausgelöst, wenn die Hauptfunktion abgeschaltet werden soll
  Delegate<bool>* CanActivate;              // Gibt an, ob die Hauptfunktion aktiviert werden kann
private:
  NkkKey* FunctionKey;                      // Taster zum Steuern der Hauptfunktion
  bool IsSwitching;                         // Gibt an, ob gerade geschaltet wird
  Timer SwitchTimeout;                      // Löst nach einer gewissen Zeit einen Timeout aus
  KnxObjectData SendObj;                    // KNX Objekt, an das der Schaltzustand gesendet wird
  LinkedList<KnxObjectData*>* RecvObj;      // KNX Objekte, die Rückmeldung geben sollen
  
  // Konstruktor
public:
  MainFunction(NkkKey* functionKey, const byte functionKnxObjectIndex,
    const __FlashStringHelper* name, unsigned int timeout = TIME_MAINFUNCTION_TIMEOUT);

  // Methoden
public:
  // Initialisiert die Hauptfunktion
  void Begin();

  // Muss in jedem Frame aufgerufen werden
  void Update();

  // Fügt eine Rückmeldung hinzu
  void AddRecvObj(const byte functionKnxObjectIndex, const __FlashStringHelper* name);

  // Schaltet diese Hauptfunktion aus
  void SwitchOff();

  // Gibt an ob die Hauptfunktion komplett eingeschaltet ist
  bool IsOn();

private:
  // Wird aufgerufen, wenn der Taster lang gedrückt wird
  void KeyLongPressed();

  // Wird aufgerufen um zu schalten
  void SwitchFunction(bool newValue);

  // Wird aufgerufen, wenn eine Rückmeldung empfangen wurde
  void FunctionKnxObjectReceived(byte object, char* value);

  // Wird aufgerufen, wenn der Schaltvorgang ausgewertet werden soll (Korrektheit, Zurückschalten)
  void Evaluate(bool generateErrors = false);

  // Findet das Rückmeldeobjekt anhand der KnxObjektID
  KnxObjectData* GetRecvObject(byte ObjectIndex);

};

#endif
