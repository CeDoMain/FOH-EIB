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

    digitalWrite(LED_BUILTIN, millis() % 1000 < 500 ? HIGH : LOW);
    Global::Update();
    m->Update();

    Global::LoopFps = 1000.0 / ( millis() - TimeStamp);
}
