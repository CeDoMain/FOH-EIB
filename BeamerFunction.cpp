#include "BeamerFunction.h"

BeamerFunction::BeamerFunction(NkkKey* functionKey)
    : CanActivate(0), FunctionKey(functionKey), State(BeamerStates::Off), IsSwitchedOn(false),
    SwitchTimeout(TIME_BEAMER_TIMEOUT),
    GetStateTimer(TIME_BEAMER_GETSTATE),
    ErrorID(Global::Disp.RegisterError(TEXT_BEAMER))
{
  
}

void BeamerFunction::Begin()
{
    // Led initialisieren
    FunctionKey->Led.SetRatio(BiColorLED::RG_Red);
    FunctionKey->Led.On();

    // Events für Timer, Taster und KNX verknüpfen
    FunctionKey->Btn.LongPressEvent = new Delegate<>(this, &BeamerFunction::KeyLongPressed);
    SwitchTimeout.TimeIsUpEvent = new Delegate<>(this, &BeamerFunction::Evaluate);
    GetStateTimer.TimeIsUpEvent = new Delegate<>(this, &BeamerFunction::GetState);
    SIMKNX128::AnyValueRecvEvent.Connect(this, &BeamerFunction::FunctionKnxObjectReceived);
}

void BeamerFunction::Update()
{
    SwitchTimeout.Update();
    GetStateTimer.Update();
}

void BeamerFunction::SwitchOff()
{
    // Wenn die Hauptfunktion eingeschaltet ist wird sie ausgeschaltet
    if (IsSwitchedOn)
      SwitchFunction(false);
}

void BeamerFunction::KeyLongPressed()
{
    // Status prüfen
    if (State == BeamerStates::WarmUp || State == BeamerStates::CoolDown)
    {
        // Kein Umschalten möglich, denn Beamer schaltet bereits
        Global::Disp.ShowMessage(TEXT_ALREADY_SWITCHING_1, TEXT_ALREADY_SWITCHING_2);
        return;
    }
    if (CanActivate != 0 && !(*CanActivate)())
    {
        // Kein Umschalten möglich, da die Masterfunktion nicht aktiv ist
        return;
    }
    if (!Global::IsKeyLockInactive)
    {
        // Kein Umschalten möglich, da der Schlüssel nicht aktiv ist
        Global::Disp.ShowMessage(TEXT_KEY, TEXT_NOT_ACTIVE);
        return;
    }
    
    // Umschalten
    SwitchFunction(!IsSwitchedOn);
}

void BeamerFunction::SwitchFunction(bool newValue)
{
    // Schaltzustand ändern
    IsSwitchedOn = newValue;
    Global::Disp.ShowMessage(TEXT_BEAMER,
        IsSwitchedOn ? TEXT_SWITCHING_ON : TEXT_SWITCHING_OFF);
    
    // LED mit neuer Farbe pulsieren lassen
    FunctionKey->Led.SetRatio(IsSwitchedOn ? BiColorLED::RG_Green : BiColorLED::RG_Red);
    FunctionKey->Led.SetPulsePerSecond(TIME_PULSERATE);
    
    // KNX-Schaltbefehl senden und Timeout starten
    SwitchTimeout.Start();
    Global::Knx.SendBool(KNX_BEAMER_SWITCH, IsSwitchedOn);

    // Statuspoll starten
    GetStateTimer.Start();
}

void BeamerFunction::GetState()
{
    // KNX Statusabfrage auslösen
    Global::Knx.SendBool(KNX_BEAMER_GETSTATE, true);
    GetStateTimer.Start();
}

void BeamerFunction::FunctionKnxObjectReceived(byte object, char* value)
{
    // Nur wahre Rückmeldungen zulassen
    if (!SIMKNX128::ParseBool(value))
        return;

    BeamerStates newState = State;

    // neuen Status auslesen
    if (object == KNX_BEAMER_OFF)
        newState = BeamerStates::Off;
    else if (object == KNX_BEAMER_WARMUP)
        newState = BeamerStates::WarmUp;
    else if (object == KNX_BEAMER_ON)
        newState = BeamerStates::On;
    else if (object == KNX_BEAMER_COOLDOWN)
        newState = BeamerStates::CoolDown;
    else
        return;

    // Status verarbeiten
    if (State != newState || newState == BeamerStates::Off || newState == BeamerStates::On)
    {
        State = newState;
        switch (State)
        {
            // Bei fertigem Schaltvorgang Rückmeldungen auswerten
            case On:
            case Off: Evaluate(); break;

            // Mitteilungen anzeigen
            case WarmUp: Global::Disp.ShowMessage(TEXT_BEAMER_NOTIFY, TEXT_BEAMER_WARMUP); break;
            case CoolDown: Global::Disp.ShowMessage(TEXT_BEAMER_NOTIFY, TEXT_BEAMER_COOLDOWN); break;
        }
    }
}

void BeamerFunction::Evaluate()
{
    // Timeouts anhalten
    SwitchTimeout.Stop();
    GetStateTimer.Stop();

    // Schaltstatus prüfen (für diesen ganzen Block gibts ein TODO zur Optimierung)
    if (State == BeamerStates::On)
    {
        Global::Disp.ShowMessage(TEXT_BEAMER, TEXT_ON);
    }
    else if (State == BeamerStates::Off)
    {
        Global::Disp.ShowMessage(TEXT_BEAMER, TEXT_OFF);
    }
    else
    {
        // Timeout vor erfolgreichem Umschalten
        Global::Disp.ErrorOccured(ErrorID);
        IsSwitchedOn = !IsSwitchedOn;
        FunctionKey->Led.SetRatio(IsSwitchedOn ? BiColorLED::RG_Green : BiColorLED::RG_Red);
        State = IsSwitchedOn ? BeamerStates::On : BeamerStates::Off;
    }
    FunctionKey->Led.SetPulsePerSecond(0);
}
