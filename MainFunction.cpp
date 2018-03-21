#include "MainFunction.h"

  MainFunction::MainFunction(NkkKey* functionKey, const byte functionKnxObjectIndex, const __FlashStringHelper* name, unsigned long timeout)
    : CanActivate(0), FunctionKey(functionKey), IsSwitching(false),
    SwitchTimeout(timeout),
    SendObj({ functionKnxObjectIndex, false, true, name, Global::Disp.RegisterError(name)}),
    RecvObj(new LinkedList<KnxObjectData*>())
{

}

void MainFunction::Begin()
{
    // Led initialisieren
    FunctionKey->Led.SetRatio(BiColorLED::RG_Red);
    FunctionKey->Led.On();

    // Delegates verknüpfen
    FunctionKey->Btn.LongPressEvent = new Delegate<>(this, &MainFunction::KeyLongPressed);
    SIMKNX128::AnyValueRecvEvent.Connect(this, &MainFunction::FunctionKnxObjectReceived);
    SwitchTimeout.TimeIsUpEvent = new Delegate<>([this]() { Evaluate(); });
}

void MainFunction::Update()
{
    SwitchTimeout.Update();
}

void MainFunction::AddRecvObj(const byte functionKnxObjectIndex, const __FlashStringHelper* name)
{
    // Wrapper für einfacheren Code in der Main.cpp
    RecvObj->Add(new KnxObjectData{ functionKnxObjectIndex, false, true, name, Global::Disp.RegisterError(name)});
}

void MainFunction::SwitchOff()
{
    // Wenn die Hauptfunktion eingeschaltet ist wird sie ausgeschaltet
    if (SendObj.Value)
        SwitchFunction(false);
}

bool MainFunction::IsOn()
{
    return SendObj.Value && !IsSwitching;
}

void MainFunction::KeyLongPressed()
{
    // Status prüfen
    if (IsSwitching)
    {
        // Kein Umschalten möglich, denn Hauptfunktion schaltet bereits
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
    SwitchFunction(!SendObj.Value);
}

void MainFunction::SwitchFunction(bool newValue)
{
    // Schaltzustand ändern
    IsSwitching = true;
    SendObj.Value = newValue;
    if (SendObj.Name != 0)
        Global::Disp.ShowMessage(SendObj.Name, SendObj.Value ? TEXT_SWITCHING_ON : TEXT_SWITCHING_OFF);
    
    // LED mit neuer Farbe pulsieren lassen
    FunctionKey->Led.SetRatio(SendObj.Value ? BiColorLED::RG_Green : BiColorLED::RG_Red);
    FunctionKey->Led.SetPulsePerSecond(TIME_PULSERATE);
    SwitchTimeout.Start();
    if (!newValue)
        SwitchingOffEvent();
    
    // Alle Rückmeldungen ungültig machen (damit bei der Auswertung Fehlende erkannt werden)
    RecvObj->ForEach([](KnxObjectData* obj) { obj->IsCorrectValue = false; });
    
    // KNX-Schaltbefehl senden und Timeout starten
    Global::Knx.SendBool(SendObj.FunctionKnxObjectIndex, SendObj.Value);
}

void MainFunction::FunctionKnxObjectReceived(byte object, char* value)
{
    KnxObjectData* obj = GetRecvObject(object);
    if (obj != 0)
    {
        // Rückmeldung speichern und auf Korrektheit prüfen
        obj->Value = SIMKNX128::ParseBool(value);
        obj->IsCorrectValue = obj->Value == SendObj.Value;

        // Meldung oder Fehler anzeigen
        if (obj->IsCorrectValue)
        {
            if(obj->Name != 0)
                Global::Disp.ShowMessage(obj->Name,
                  obj->Value ? TEXT_ON : TEXT_OFF);
        }
        else
        {
            Global::Disp.ErrorOccured(obj->ErrorID);
        }

        // Tritt ein Fehler nicht beim Schalten auf, muss die Hauptfunktion neu ausgewertet werden
        if (!obj->IsCorrectValue && !IsSwitching)
            Evaluate(true);

        // Wenn alle Rückmeldungen richtig sind, kann die Hauptfunktion bereits ausgewertet werden
        bool AllCorrect = true;
        RecvObj->ForEach([&AllCorrect](KnxObjectData* obj) { if (!obj->IsCorrectValue) AllCorrect = false; });
        if (AllCorrect)
            Evaluate();
    }
}

void MainFunction::Evaluate(bool generateErrors)
{
    // Timout stoppen
    SwitchTimeout.Stop();

    // Prüfen ob Fehler vorliegen
    bool AllCorrect = true;
    bool AnyCorrect = false;
    RecvObj->ForEach([&AllCorrect, &AnyCorrect](KnxObjectData* obj)
    {
        if (obj->IsCorrectValue)
            AnyCorrect = true;
        else
            AllCorrect = false;
    });
    if (AllCorrect)
    {
        // Alle Rückmeldungen korrekt
        if (SendObj.Name != 0)
            Global::Disp.ShowMessage(SendObj.Name, SendObj.Value ? TEXT_ON : TEXT_OFF);
    }
    else if (AnyCorrect)
    {
        // Rückmeldungen teilweise korrekt
        if (!generateErrors)
        {
            // Für jede Fehlende Rückmeldung einen Fehler anzeigen
            RecvObj->ForEach([](KnxObjectData* obj)
            {
                if (!obj->IsCorrectValue)
                    Global::Disp.ErrorOccured(obj->ErrorID);
            });
        }
    }
    else
    {
        // Keine Rückmeldung korrekt (Umschalten fehlgeschlagen)
        Global::Disp.ErrorOccured(SendObj.ErrorID);
        SendObj.Value = !SendObj.Value;
        FunctionKey->Led.SetRatio(SendObj.Value ? BiColorLED::RG_Green : BiColorLED::RG_Red);
    }

    // LED dauerleuchten lassen und Meldung auslösen
    FunctionKey->Led.SetPulsePerSecond(0);
    IsSwitching = false;
    if (!SendObj.Value)
        SwitchingOffEvent();
}

MainFunction::KnxObjectData* MainFunction::GetRecvObject(byte ObjectIndex)
{
    // Zum KNX Objekt passende Rückmeldung finden
    return RecvObj->Find([ObjectIndex](KnxObjectData* data) -> bool
        { return data->FunctionKnxObjectIndex == ObjectIndex; });
}
