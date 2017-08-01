#include "MainFunction.h"

  MainFunction::MainFunction(NkkKey* functionKey, const byte functionKnxObjectIndex, const __FlashStringHelper* name)
  : CanActivate(0), FunctionKey(functionKey), IsSwitching(false),
    SwitchTimeout(TIME_MAINFUNCTION_TIMEOUT),
    SendObj({ functionKnxObjectIndex, false, true, name, Global::Disp.RegisterError(name)}),
    RecvObj(new LinkedList<KnxObjectData*>()),
    PulsePerSecond(TIME_PULSERATE)
{

}

void MainFunction::Begin()
{
  // Led initialisieren
  FunctionKey->Led.SetRatio(BiColorLED::RG_Red);
  FunctionKey->Led.On();

  // Delegates verknüpfen
  FunctionKey->Btn.LongPressEvent.Connect(this, &MainFunction::KeyLongPressed);
  SIMKNX128::AnyValueRecvEvent.Connect(this, &MainFunction::FunctionKnxObjectReceived);
  SwitchTimeout.TimeIsUpEvent.Connect(this, &MainFunction::Evaluate);
}

void MainFunction::Update()
{
  SwitchTimeout.Update();
}

void MainFunction::AddRecvObj(const byte functionKnxObjectIndex, const __FlashStringHelper* name)
{
  RecvObj->Add(new KnxObjectData{ functionKnxObjectIndex, false, true, name, Global::Disp.RegisterError(name)});
}

void MainFunction::SwitchOff()
{
  if (SendObj.Value)
    SwitchFunction(false);
}

bool MainFunction::IsOn()
{
  return SendObj.Value && !IsSwitching;
}

void MainFunction::KeyLongPressed()
{
  if (IsSwitching)
  {
    Global::Disp.ShowMessage(TEXT_ALREADY_SWITCHING_1, TEXT_ALREADY_SWITCHING_2);
    return;
  }
  if (CanActivate != 0 && !(*CanActivate)())
  {
    return;
  }
  if (!Global::IsKeyLockInactive)
  {
    Global::Disp.ShowMessage(TEXT_KEY, TEXT_NOT_ACTIVE);
    return;
  }
  SwitchFunction(!SendObj.Value);
}

void MainFunction::SwitchFunction(bool newValue)
{
  IsSwitching = true;
  SendObj.Value = newValue;
  if (SendObj.Name != 0)
    Global::Disp.ShowMessage(SendObj.Name, SendObj.Value ? TEXT_SWITCHING_ON : TEXT_SWITCHING_OFF);
  FunctionKey->Led.SetRatio(SendObj.Value ? BiColorLED::RG_Green : BiColorLED::RG_Red);
  FunctionKey->Led.SetPulsePerSecond(PulsePerSecond);
  SwitchTimeout.Start();
  if (!newValue)
    SwitchingOffEvent();
  RecvObj->ForEach([](KnxObjectData* obj) { obj->IsCorrectValue = false; });
  Global::Knx.SendBool(SendObj.FunctionKnxObjectIndex, SendObj.Value);
}

void MainFunction::FunctionKnxObjectReceived(byte object, char* value)
{
  KnxObjectData* obj = GetRecvObject(object);
  if (obj != 0)
  {
    // Rückmeldung speichern und auf Korrektheit prüfen
    obj->Value = SIMKNX128::ParseBool(value);
    obj->IsCorrectValue = SIMKNX128::ParseBool(value) == SendObj.Value;

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
      Evaluate();

    // Wenn alle Rückmeldungen richtig sind, kann die Hauptfunktion bereits ausgewertet werden
    bool AllCorrect = true;
    RecvObj->ForEach([&AllCorrect](KnxObjectData* obj) { if (!obj->IsCorrectValue) AllCorrect = false; });
    if (AllCorrect)
      Evaluate();
  }
}

void MainFunction::Evaluate()
{
  SwitchTimeout.Stop();
  bool AllCorrect = true;
  bool AnyCorrect = false;
  RecvObj->ForEach([&AllCorrect, &AnyCorrect](KnxObjectData* obj)
    {
      if (obj->IsCorrectValue)
        AnyCorrect = true;
      else
        AllCorrect = false;
    });
  if(AllCorrect)
  {
    if (SendObj.Name != 0)
      Global::Disp.ShowMessage(SendObj.Name, SendObj.Value ? TEXT_ON : TEXT_OFF);
  }
  else if (AnyCorrect)
  {
    RecvObj->ForEach([](KnxObjectData* obj)
    {
      if (!obj->IsCorrectValue)
        Global::Disp.ErrorOccured(obj->ErrorID);
    });
  }
  else
  {
    Global::Disp.ErrorOccured(SendObj.ErrorID);
    SendObj.Value = !SendObj.Value;
    FunctionKey->Led.SetRatio(SendObj.Value ? BiColorLED::RG_Green : BiColorLED::RG_Red);
  }
  FunctionKey->Led.SetPulsePerSecond(0);
  IsSwitching = false;
  if (!SendObj.Value)
    SwitchingOffEvent();
}

MainFunction::KnxObjectData* MainFunction::GetRecvObject(byte ObjectIndex)
{
  return RecvObj->Find([ObjectIndex](KnxObjectData* data) -> bool
    { return data->FunctionKnxObjectIndex == ObjectIndex; });
}
