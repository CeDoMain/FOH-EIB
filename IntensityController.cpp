#include "IntensityController.h"

IntensityController::IntensityController(NkkKey* intensityKey)
    : IntensityKey(intensityKey), DimmerTimeout(TIME_DIMM_TIMEOUT), Fade(),
    ActiveIntensity(LGHT_DIMM_ACTIVEINTENSITY)
{

}

void IntensityController::Begin()
{
    // Events verknüpfen
    DimmerTimeout.TimeIsUpEvent = new Delegate<>(this, &IntensityController::DimmIntensity);
    IntensityKey->Btn.ClickEvent = new Delegate<>(this, &IntensityController::SwitchIntensity);
    Fade.FadeEvent = new Delegate<void, decimal>(this, &IntensityController::FadeChanged);
    Trigger::AnyActiveEvent.Connect(this, &IntensityController::WakeUp);

    // Taster und Dimmtimer initialisieren
    BiColorLED::SetGlobalIntensity(0);
    Fade.Start(TIME_DIMM_WAKEUPTIME, 0, ActiveIntensity);
    IntensityKey->Led.SetRatio(BiColorLED::RG_Yellow);
    IntensityKey->Led.On();
    DimmerTimeout.Start();
}

void IntensityController::Update()
{
    DimmerTimeout.Update();
    Fade.Update();
}

void IntensityController::FadeChanged(decimal value)
{
    // Taster- und Displayhelligkeit anpassen
    BiColorLED::SetGlobalIntensity(value);
    Global::Disp.SetIntensity(value / 10000.0);
}

void IntensityController::DimmIntensity()
{
    // Auf Minimalhelligkeit dimmen
    Fade.Start(TIME_DIMM_DIMMTIME, LGHT_DIMM_DIMMEDINTENSITY);
}

void IntensityController::WakeUp()
{
    // Auf eingestellte Arbeitshelligkeit hochdimmen
    Fade.Start(TIME_DIMM_WAKEUPTIME, ActiveIntensity);
    DimmerTimeout.Start();
}

void IntensityController::SwitchIntensity()
{
    // Arbeitshelligkeit verstellen
    ActiveIntensity += 4000;
    if(ActiveIntensity > 10000)
        ActiveIntensity = 2000;
    Fade.Start(TIME_DIMM_WAKEUPTIME, ActiveIntensity);
    Global::Disp.ShowIntensity((ActiveIntensity - 2000) / 4000 + 1, 3);
}
