#include "BeamerFunction.h"

BeamerFunction::BeamerFunction(NkkKey* functionKey)
  : CanActivate(0), FunctionKey(functionKey), State(BeamerStates::Ready), IsSwitchedOn(false),
    SendSwitch(KNX_BEAMER_SWITCH), SendGetState(KNX_BEAMER_GETSTATE),
    RecvWarmUp(KNX_BEAMER_WARMUP), RecvReady(KNX_BEAMER_READY), RecvCountDown(KNX_BEAMER_COUNTDOWN), RecvCoolDown(KNX_BEAMER_COOLDOWN),
    PulsePerSecond(TIME_PULSERATE), SwitchTimeout(TIME_BEAMER_TIMEOUT),
    GetStateTimer(TIME_BEAMER_GETSTATE, true),
    ErrorID(Global::Disp.RegisterError(TEXT_BEAMER))
{

}

void BeamerFunction::Begin()
{
  // Led initialisieren
  FunctionKey->Led.SetRatio(BiColorLED::RG_Red);
  FunctionKey->Led.On();

  // Delegates verknüpfen
  FunctionKey->Btn.LongPressEvent.Connect(this, &BeamerFunction::KeyLongPressed);
  SIMKNX128::AnyValueRecvEvent.Connect(this, &BeamerFunction::FunctionKnxObjectReceived);
  SwitchTimeout.TimeIsUpEvent.Connect(this, &BeamerFunction::Evaluate);
  GetStateTimer.TimeIsUpEvent.Connect(this, &BeamerFunction::GetState);
}

void BeamerFunction::Update()
{
  SwitchTimeout.Update();
  GetStateTimer.Update();
}

void BeamerFunction::SwitchOff()
{
  if (IsSwitchedOn)
    SwitchFunction(false);
}

void BeamerFunction::KeyLongPressed()
{
  if (State != BeamerStates::Ready)
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
  SwitchFunction(!IsSwitchedOn);
}

void BeamerFunction::SwitchFunction(bool newValue)
{
IsSwitchedOn = newValue;
Global::Disp.ShowMessage(TEXT_BEAMER,
  IsSwitchedOn ? TEXT_SWITCHING_ON : TEXT_SWITCHING_OFF);
FunctionKey->Led.SetRatio(IsSwitchedOn ? BiColorLED::RG_Green : BiColorLED::RG_Red);
FunctionKey->Led.SetPulsePerSecond(PulsePerSecond);
SwitchTimeout.Start();
Global::Knx.SendBool(SendSwitch, IsSwitchedOn);
GetStateTimer.Start();
}

void BeamerFunction::GetState()
{
  Global::Knx.SendBool(SendGetState, true);
}

void BeamerFunction::FunctionKnxObjectReceived(byte object, char* value)
{
  // Nur wahre Rückmeldungen zulassen
  if (!SIMKNX128::ParseBool(value))
    return;

  BeamerStates newState = State;

  if (object == RecvWarmUp)
    newState = BeamerStates::WarmUp;
  else if (object == RecvReady)
    newState = BeamerStates::Ready;
  else if (object == RecvCountDown)
    newState = BeamerStates::CountDown;
  else if (object == RecvCoolDown)
    newState = BeamerStates::CoolDown;
  else
    return;

  if (State != newState)
  {
    State = newState;
    switch (State)
    {
      case WarmUp: Global::Disp.ShowMessage(TEXT_BEAMER_NOTIFY, TEXT_BEAMER_BOOTING); break;
      case Ready: Evaluate(); break;
      case CountDown: Global::Disp.ShowMessage(TEXT_BEAMER_NOTIFY, TEXT_BEAMER_COUNTDOWN); break;
      case CoolDown: Global::Disp.ShowMessage(TEXT_BEAMER_NOTIFY, TEXT_BEAMER_COOLDOWN); break;
    }
  }
}

void BeamerFunction::Evaluate()
{
  SwitchTimeout.Stop();
  GetStateTimer.Stop();
  if(State == BeamerStates::Ready)
  {
    Global::Disp.ShowMessage(TEXT_BEAMER, IsSwitchedOn ? TEXT_ON : TEXT_OFF);
  }
  else
  {
    Global::Disp.ErrorOccured(ErrorID);
    IsSwitchedOn = !IsSwitchedOn;
    FunctionKey->Led.SetRatio(IsSwitchedOn ? BiColorLED::RG_Green : BiColorLED::RG_Red);
  }
  FunctionKey->Led.SetPulsePerSecond(0);
  State = BeamerStates::Ready;
}
