#include "Main.h"

Main* m;
using Error = DisplayController::Error;

#ifdef DEBUG
  Timer MemoryPrint(4000, true, true);

void PrintRAM()
{
  double mem = 100.0 - freeMemory() / 8186.0 * 100.0;
  Debug(F("%i.%i%% RAM used\n"), (int)mem, (int)((mem - (int)mem) * 100));
}
#endif

void setup()
{
  #ifdef DEBUG
    Serial.begin(9600);
    Debug(F("Setup begin\n"));
  #endif

  pinMode(LED_BUILTIN, OUTPUT);
  Global::Begin();
  m = new Main();
  m->Begin();

  #ifdef DEBUG
    PrintRAM();
    MemoryPrint.TimeIsUpEvent.Connect(&PrintRAM);
    Debug(F("Setup end\n"));
  #endif
}

void loop()
{
  //static unsigned long TimeStamp = 0;
  //TimeStamp = millis();
  analogWrite(LED_BUILTIN, 128 + sin(millis() / 1000.0 * 3.14) * 128);
  Global::Update();
  m->Update();

  #ifdef DEBUG
    MemoryPrint.Update();
  #endif
  //Serial.println(1000.0 / ( millis() - TimeStamp));
}

