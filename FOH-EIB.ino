#include "Main.h"

Main* m;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  Global::Begin();
  m = new Main();
  m->Begin();
}

void loop()
{
  static unsigned long TimeStamp = 0;
  TimeStamp = millis();

  analogWrite(LED_BUILTIN, 128 + sin(millis() / 1000.0 * 3.14) * 128);
  Global::Update();
  m->Update();

  Global::LoopFps = 1000.0 / ( millis() - TimeStamp);
}
