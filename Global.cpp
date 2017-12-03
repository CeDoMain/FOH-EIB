#include "Global.h"

PCA9685 Global::PCMchip[5] =
{
  B111000 | CONN_I2C_CONTROL,
  B111000 | CONN_I2C_MAINFUNC_1,
  B111000 | CONN_I2C_MAINFUNC_2,
  B111000 | CONN_I2C_WINDOW_1,
  B111000 | CONN_I2C_WINDOW_2
};
MCP23017 Global::GPIOchip[5] =
{
  CONN_I2C_CONTROL,
  CONN_I2C_MAINFUNC_1,
  CONN_I2C_MAINFUNC_2,
  CONN_I2C_WINDOW_1,
  CONN_I2C_WINDOW_2
};
NkkKey Global::Key[5][5] =
{
 {
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() }
 },
 {
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() }
 },
 {
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() }
 },
 {
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() }
 },
 {
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() },
   { Button(), BiColorLED() }
 }
};
bool Global::IsKeyLockInactive = false;
unsigned int Global::LoopFps = 0;
DisplayController Global::Disp = DisplayController(&KEY_ERROR, &Global::IsKeyLockInactive);
SIMKNX128 Global::Knx = SIMKNX128();
Button Global::KeyLockBtn = Button();

void Global::Begin()
{
  // Hardware initialisieren
  for (int y = 0; y < 5; y++)
  {
    PCMchip[y].Begin();
    GPIOchip[y].Begin();
  }

  // Taster, Trigger und LEDs miteinander verknüpfen
  KeyLockBtn.Begin(new Trigger(ArduinoMega::GetDigitalIn(CONN_KEY)));
  for (int y = 0; y < 5; y++)
  {
    Key[y][0].Btn.Begin(new Trigger(GPIOchip[y].GetDigitalIn(MCP23017::GPA1)));
    Key[y][1].Btn.Begin(new Trigger(GPIOchip[y].GetDigitalIn(MCP23017::GPA2)));
    Key[y][2].Btn.Begin(new Trigger(GPIOchip[y].GetDigitalIn(MCP23017::GPA7)));
    Key[y][3].Btn.Begin(new Trigger(GPIOchip[y].GetDigitalIn(MCP23017::GPA6)));
    Key[y][4].Btn.Begin(new Trigger(GPIOchip[y].GetDigitalIn(MCP23017::GPA4)));
    Key[y][0].Led.Begin(PCMchip[y].GetAnalogOut(PCA9685::LED13), PCMchip[y].GetAnalogOut(PCA9685::LED12));
    Key[y][1].Led.Begin(PCMchip[y].GetAnalogOut(PCA9685::LED11), PCMchip[y].GetAnalogOut(PCA9685::LED10));
    Key[y][2].Led.Begin(PCMchip[y].GetAnalogOut(PCA9685::LED7), PCMchip[y].GetAnalogOut(PCA9685::LED6));
    Key[y][3].Led.Begin(PCMchip[y].GetAnalogOut(PCA9685::LED5), PCMchip[y].GetAnalogOut(PCA9685::LED4));
    Key[y][4].Led.Begin(PCMchip[y].GetAnalogOut(PCA9685::LED1), PCMchip[y].GetAnalogOut(PCA9685::LED0));
  }
  Disp.Begin();
  Knx.Begin();
  KeyLockBtn.LongPressEvent = new Delegate<>(&Global::KeyLockActivate);
  KeyLockBtn.DeactivatedEvent = new Delegate<>(&Global::KeyLockDeactivate);
  KEY_FOHLIGHT.Led.SetRatio(BiColorLED::RG_Orange);
  KEY_FOHLIGHT.Led.SetIntensity(10000);
  KEY_FOHLIGHT.Led.On();
  KEY_INFO.Led.SetRatio(BiColorLED::RG_Orange);
  KEY_INFO.Led.SetIntensity(10000);
  KEY_INFO.Led.On();
  KEY_FOHLIGHT.Btn.LongPressEvent = new Delegate<>([]()
  {
    static bool LightOn = false;
    LightOn = !LightOn;
    Global::Knx.SendBool(KNX_GLOBAL_LIGHTFOH, LightOn);
    Global::Disp.ShowMessage(F("Licht unter FOH"), LightOn ? TEXT_ON : TEXT_OFF);
  });
  KEY_INFO.Btn.ClickEvent = new Delegate<>([]()
  {
    float mem = 100.0 - freeMemory() / 8186.0 * 100.0;

    char line1[20];
    char line2[20];
    sprintf(line1, "RAM: %i.%i%% %ifps", (int)mem, (int)((mem - (int)mem) * 100), Global::LoopFps);
    sprintf(line2, "Version: 2.7");
    Global::Disp.DumpErrorList();
    Global::Disp.ShowMessage(line1, line2);
    Serial.println(line1);
    Serial.println(line2);
  });
}

void Global::Update()
{
  // Hardware updaten
  for (int y = 0; y < 5; y++)
  {
    GPIOchip[y].Update();
  }

  // Trigger und LEDs updaten
  KeyLockBtn.Update();
  for (int y = 0; y < 5; y++)
  {
    for (int x = 0; x < 5; x++)
    {
      Key[y][x].Btn.Update();
      Key[y][x].Led.Update();
    }
  }
  Knx.Update();
  Disp.Update();
}

void Global::KeyLockActivate()
{
  IsKeyLockInactive = true;
  Disp.ShowMessage(TEXT_KEY, TEXT_ACTIVATED);
}

void Global::KeyLockDeactivate()
{
  IsKeyLockInactive = false;
  Disp.ShowMessage(TEXT_KEY, TEXT_DEACTIVATED);
}
