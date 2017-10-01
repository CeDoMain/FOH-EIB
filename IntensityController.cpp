#include "IntensityController.h"

IntensityController::IntensityController(NkkKey* intensityKey)
  : IntensityKey(intensityKey), DimmerTimeout(TIME_DIMM_TIMEOUT), Fade(),
    ActiveIntensity(LGHT_DIMM_ACTIVEINTENSITY), DimmedIntensity(LGHT_DIMM_DIMMEDINTENSITY),
    DimmTime(TIME_DIMM_DIMMTIME), WakeUpTime(TIME_DIMM_WAKEUPTIME)
{

}

void IntensityController::Begin()
{
  // Events verknüpfen
  DimmerTimeout.TimeIsUpEvent = new Delegate<>(this, &IntensityController::DimmIntensity);
  IntensityKey->Btn.ClickEvent = new Delegate<>(this, &IntensityController::SwitchIntensity);
  Fade.FadeEvent = new Delegate<void, float>(this, &IntensityController::FadeChanged);
  Trigger::AnyActiveEvent.Connect(this, &IntensityController::WakeUp);

  BiColorLED::SetGlobalIntensity(0);
  Fade.Start(WakeUpTime, 0, ActiveIntensity);
  IntensityKey->Led.SetRatio(BiColorLED::RG_Yellow);
  IntensityKey->Led.On();
  DimmerTimeout.Start();
}

void IntensityController::Update()
{
  DimmerTimeout.Update();
  Fade.Update();
}

void IntensityController::FadeChanged(float value)
{
  BiColorLED::SetGlobalIntensity(value);
  Global::Disp.SetIntensity(value);
}

void IntensityController::DimmIntensity()
{
  Fade.Start(DimmTime, DimmedIntensity);
}

void IntensityController::WakeUp()
{
  Fade.Start(WakeUpTime, ActiveIntensity);
  DimmerTimeout.Start();
}

void IntensityController::SwitchIntensity()
{
  // Helligkeit verstellen
  ActiveIntensity += 0.4;
  if(ActiveIntensity > 1)
    ActiveIntensity = 0.2;
  Fade.Start(WakeUpTime, ActiveIntensity);
  Global::Disp.ShowIntensity((ActiveIntensity - 0.2) / 0.4 + 1, 3);
}
