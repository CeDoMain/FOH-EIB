#include "IntensityController.h"

IntensityController::IntensityController(NkkKey* intensityKey)
  : IntensityKey(intensityKey), DimmerTimeout(TIME_DIMM_TIMEOUT), Fade(),
    Contrast(LGHT_DIMM_CONTRAST), ActiveIntensity(LGHT_DIMM_ACTIVEINTENSITY), DimmedIntensity(LGHT_DIMM_DIMMEDINTENSITY),
    DimmTime(TIME_DIMM_DIMMTIME), WakeUpTime(TIME_DIMM_WAKEUPTIME)
{

}

void IntensityController::Begin()
{
  // Events verknüpfen
  DimmerTimeout.TimeIsUpEvent.Connect(this, &IntensityController::DimmIntensity);
  IntensityKey->Btn.ClickEvent.Connect(this, &IntensityController::SwitchIntensity);
  Trigger::AnyActiveEvent.Connect(this, &IntensityController::WakeUp);
  Fade.FadeEvent.Connect(this, &IntensityController::FadeChanged);

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
  if (Global::IsKeyLockInactive)
  {
    // Kontrast verstellen
    Contrast += 0.1;
    if(Contrast >= 1)
      Contrast = 0;
    Global::Disp.SetContrast(Contrast);
    Global::Disp.ShowContrast(Contrast / 0.1, 9);
  }
  else
  {
    // Helligkeit verstellen
    ActiveIntensity += 0.4;
    if(ActiveIntensity > 1)
      ActiveIntensity = 0.2;
    Fade.Start(WakeUpTime, ActiveIntensity);
    Global::Disp.ShowIntensity((ActiveIntensity - 0.2) / 0.4 + 1, 3);
  }
}
