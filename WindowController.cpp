#include "WindowController.h"

WindowController::WindowController(NkkKey* upKey, NkkKey* downKey)
  : UpKey(upKey), DownKey(downKey), Windows(new LinkedList<KnxObjectData*>()),
    SelectedWindow(0), DeselectTimer(TIME_WINDOW_DESELECT)
{

}
void WindowController::Begin()
{
  UpKey->Led.SetIntensity(10000);
  UpKey->Led.Off();

  DownKey->Led.SetIntensity(10000);
  DownKey->Led.Off();
  Windows->ForEach([this](KnxObjectData* obj)
    {
      obj->Key->Led.SetRatio(BiColorLED::RG_Yellow);
      obj->Key->Led.SetIntensity(10000);
      obj->Key->Led.On();
      obj->Key->Btn.ClickEvent = new Delegate<>([this, obj]() { Select(obj); });
      Global::Knx.ValueRecvEvent[obj->UpTriggerKnxObjectIndex] =
        new Delegate<void, char*>([this, obj](char* value)
        {
          // Rückmeldung über oberen Anschlag
          obj->Key->Led.SetRatio(
            SIMKNX128::ParseBool(value)
            ? (obj->IsColorReversed ? BiColorLED::RG_Red : BiColorLED::RG_Green)
            : BiColorLED::RG_Yellow);
        });
      Global::Knx.ValueRecvEvent[obj->DownTriggerKnxObjectIndex] =
        new Delegate<void, char*>([this, obj](char* value)
        {
          // Rückmeldung über oberen Anschlag
          obj->Key->Led.SetRatio(
            SIMKNX128::ParseBool(value)
            ? (obj->IsColorReversed ? BiColorLED::RG_Green : BiColorLED::RG_Red)
            : BiColorLED::RG_Yellow);
        });
    });
  DeselectTimer.TimeIsUpEvent = new Delegate<>(this, &WindowController::Deselect);
  UpKey->Btn.ClickEvent = new Delegate<>(this, &WindowController::StepUp);
  UpKey->Btn.LongPressEvent = new Delegate<>(this, &WindowController::MoveUp);
  UpKey->Btn.ActivatedEvent = new Delegate<>([this](){ DeselectTimer.Start(); });
  DownKey->Btn.ClickEvent = new Delegate<>(this, &WindowController::StepDown);
  DownKey->Btn.LongPressEvent = new Delegate<>(this, &WindowController::MoveDown);
  DownKey->Btn.ActivatedEvent = new Delegate<>([this](){ DeselectTimer.Start(); });
}
void WindowController::Update()
{
  DeselectTimer.Update();
}
void WindowController::AddWindow(
  const byte MoveKnxObjectIndex,
  const byte StepKnxObjectIndex,
  const byte UpTriggerKnxObjectIndex,
  const byte DownTriggerKnxObjectIndex,
  const __FlashStringHelper* Name,
  NkkKey* Key,
  Delegate<bool>* CanActivate)
{
  Windows->Add(new KnxObjectData{ MoveKnxObjectIndex, StepKnxObjectIndex, UpTriggerKnxObjectIndex, DownTriggerKnxObjectIndex, Name, Key, CanActivate, true });
}
void WindowController::AddJalousie(
  const byte MoveKnxObjectIndex,
  const byte StepKnxObjectIndex,
  const byte UpTriggerKnxObjectIndex,
  const byte DownTriggerKnxObjectIndex,
  const __FlashStringHelper* Name,
  NkkKey* Key,
  Delegate<bool>* CanActivate)
{
  Windows->Add(new KnxObjectData{ MoveKnxObjectIndex, StepKnxObjectIndex, UpTriggerKnxObjectIndex, DownTriggerKnxObjectIndex, Name, Key, CanActivate, false });
}
void WindowController::Select(KnxObjectData* obj)
{
  if(obj == SelectedWindow)
  {
    // Dasselbe Fenster ist bereits ausgewählt
    DeselectTimer.Start();
    return;
  }

  if (obj->CanActivate == 0 || (*obj->CanActivate)())
  {
    // Das Fenster kann ausgewählt werden
    // Vorherige Auswahl abwählen
    Deselect();
    SelectedWindow = obj;

    // Auswahltaste blinken, die Steuertasten farbig beleuchten und Mitteilung senden
    SelectedWindow->Key->Led.SetPulsePerSecond(TIME_PULSERATE);
    UpKey->Led.SetRatio(obj->IsColorReversed ? BiColorLED::RG_Red : BiColorLED::RG_Green);
    DownKey->Led.SetRatio(obj->IsColorReversed ? BiColorLED::RG_Green : BiColorLED::RG_Red);
    UpKey->Led.On();
    DownKey->Led.On();
    Global::Disp.ShowMessage(SelectedWindow->Name, TEXT_SELECTED);
  }
  DeselectTimer.Start();
}
void WindowController::Deselect()
{
  DeselectTimer.Stop();
  if(SelectedWindow == 0)
    return;

  // Auswahltaste statisch beleuchten und die Steuertasten ausschalten
  SelectedWindow->Key->Led.SetPulsePerSecond(0);
  UpKey->Led.Off();
  DownKey->Led.Off();

  SelectedWindow = 0;
}
void WindowController::StepUp()
{
  if (SelectedWindow != 0)
  {
    DeselectTimer.Start();
    Global::Knx.SendBool(SelectedWindow->StepKnxObjectIndex, false);
    Global::Disp.ShowMessage(SelectedWindow->Name, TEXT_STEPOPEN);
  }
  else
  {
    Global::Disp.ShowMessage(TEXT_NOSELECTION);
  }
}
void WindowController::StepDown()
{
  if (SelectedWindow != 0)
  {
    DeselectTimer.Start();
    Global::Knx.SendBool(SelectedWindow->StepKnxObjectIndex, true);
    Global::Disp.ShowMessage(SelectedWindow->Name, TEXT_STEPCLOSE);
  }
  else
  {
    Global::Disp.ShowMessage(TEXT_NOSELECTION);
  }
}
void WindowController::MoveUp()
{
  if (SelectedWindow != 0)
  {
    DeselectTimer.Start();
    Global::Knx.SendBool(SelectedWindow->MoveKnxObjectIndex, false);
    Global::Disp.ShowMessage(SelectedWindow->Name, TEXT_DRIVEOPEN);
  }
  else
  {
    Global::Disp.ShowMessage(TEXT_NOSELECTION);
  }
}
void WindowController::MoveDown()
{
  if (SelectedWindow != 0)
  {
    DeselectTimer.Start();
    Global::Knx.SendBool(SelectedWindow->MoveKnxObjectIndex, true);
    Global::Disp.ShowMessage(SelectedWindow->Name, TEXT_DRIVECLOSE);
  }
  else
  {
    Global::Disp.ShowMessage(TEXT_NOSELECTION);
  }
}
