#ifndef BEAMERFUNCTION_H
#define BEAMERFUNCTION_H

#include "Global.h"
#include "BaseIO.h"
#include "Config/Texts.h"
#include "Config/Values.h"

class BeamerFunction
{
  // Felder
public:
  Delegate<bool>* CanActivate;      // Gibt an, ob die Hauptfunktion aktiviert werden kann

private:
  NkkKey* FunctionKey;              // Taster zum Steuern der Hauptfunktion
  enum BeamerStates
  {
    WarmUp,
    Ready,
    CountDown,
    CoolDown
  } State;                          // Gibt den aktuellen Zustand des Beamers an
  bool IsSwitchedOn;                // Gibt an, ob der Beamer an oder aus geschaltet ist/wird
  const byte SendSwitch;            // KNX Objekte für die verschiedenen Zustände
  const byte SendGetState;
  const byte RecvWarmUp;
  const byte RecvReady;
  const byte RecvCountDown;
  const byte RecvCoolDown;
  Timer SwitchTimeout;              // Löst nach einer gewissen Zeit einen Timeout aus
  Timer GetStateTimer;              // Fragt nach einer gewissen Zeit den Zustand des Beamers ab
  const byte ErrorID;               // ID, die den entsprechenden Fehler im DisplayController angibt

  // Konstruktor
public:
  BeamerFunction(NkkKey* functionKey);

  // Methoden
public:
  // Initialisiert die Hauptfunktion
  void Begin();

  // Muss in jedem Frame aufgerufen werden
  void Update();

  // Schaltet diese Hauptfunktion aus
  void SwitchOff();

private:
  // Wird aufgerufen, wenn der Taster lang gedrückt wird
  void KeyLongPressed();

  // Wird aufgerufen um zu schalten
  void SwitchFunction(bool newValue);

  // Wird regelmäßig aufgerufen, um den Zustand des Beamers herauszufinden
  void GetState();

  // Wird aufgerufen, wenn eine Rückmeldung empfangen wurde
  void FunctionKnxObjectReceived(byte object, char* value);

  // Wird aufgerufen, wenn ein Timeout ausgelöst wurde oder alle Rückmeldungen korrekt sind
  void Evaluate();

};

#endif
