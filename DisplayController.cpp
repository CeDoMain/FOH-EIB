#include "DisplayController.h"

DisplayController::DisplayController(NkkKey* errorKey, bool* isKeyActive)
  : Lcd(CONN_DISP_RS, CONN_DISP_RW, CONN_DISP_E,
    CONN_DISP_D0, CONN_DISP_D1, CONN_DISP_D2, CONN_DISP_D3,
    CONN_DISP_D4, CONN_DISP_D5, CONN_DISP_D6, CONN_DISP_D7),
    ErrorKey(errorKey), MessageTimeout(TIME_DISP_MSGTIMEOUT), ErrorTimeout(TIME_DISP_ERRTIMEOUT),
    Errors(), RegisteredErrorCount(0), ShowedError(-1), IsKeyActive(isKeyActive)
{

}
void DisplayController::Begin()
{
  // LCD mit 2x20 Zeichen initialisieren, leeren, Kontrast einstellen und beleuchten
  Lcd.begin(20, 2);
  SetContrast(LGHT_DIMM_CONTRAST);
  SetIntensity(0);
  Lcd.clear();

  // Events verknüpfen
  MessageTimeout.TimeIsUpEvent.Connect(this, &DisplayController::ClearScreen);
  ErrorTimeout.TimeIsUpEvent.Connect(this, &DisplayController::NextError);
  ErrorKey->Btn.ClickEvent.Connect(this, &DisplayController::NextError);
  ErrorKey->Btn.LongPressEvent.Connect(this, &DisplayController::AcknowledgeError);
  ErrorKey->Btn.ActivatedEvent.Connect(this, &DisplayController::RestartErrorTimeout);

  ErrorKey->Led.SetRatio(BiColorLED::RG_Green);
  ErrorKey->Led.On();
}
void DisplayController::Update()
{
  // Timer weiterlaufen lassen
  MessageTimeout.Update();
  ErrorTimeout.Update();
}
void DisplayController::ShowMessage(const __FlashStringHelper* msg1, const __FlashStringHelper* msg2, unsigned int intervall)
{
  // Mitteilung für eine kurze Zeit anzeigen
  MessageTimeout.Stop();
  Lcd.clear();
  Lcd.setCursor(0, 0);
  PrintTwoLine(msg1, msg2);
  MessageTimeout.SetTime(intervall);
  MessageTimeout.Start();
}
void DisplayController::ShowMessage(char* msg1, char* msg2, unsigned int intervall)
{
  // Mitteilung für eine kurze Zeit anzeigen
  MessageTimeout.Stop();
  Lcd.clear();
  Lcd.setCursor(0, 0);
  if (msg1 != 0)
  {
    Lcd.print(msg1);
  }
  if (msg2 != 0)
  {
    Lcd.setCursor(0, 1);
    Lcd.print(msg2);
  }
  MessageTimeout.SetTime(intervall);
  MessageTimeout.Start();
}
void DisplayController::ShowIntensity(byte value, byte max)
{
  // Helligkeit für eine kurze Zeit anzeigen
  MessageTimeout.Stop();
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print(TEXT_INTENSITY);
  Lcd.print(value);
  Lcd.print(F(" / "));
  Lcd.print(max);
  MessageTimeout.Start();
}
byte DisplayController::RegisterError(const __FlashStringHelper* name)
{
  byte id = RegisteredErrorCount;
  RegisteredErrorCount++;
  Errors.Add(new Error{ false, name, id });
  return id;
}
void DisplayController::ErrorOccured(byte id)
{
  // Abbrechen, wenn die ID ungültig ist
  if (id >= RegisteredErrorCount)
    return;

  // rotes Blinken aktivieren
  ErrorKey->Led.SetRatio(BiColorLED::RG_Red);
  ErrorKey->Led.SetPulsePerSecond(TIME_PULSERATE);

  // Fehler speichern
  Error* err = Errors.Find(
    [id](Error* err) -> bool { return err->ID == id; });
  err->Occurred = true;

  // Fehler anzeigen
  ShowedError = id;
  ShowError();
}
void DisplayController::SetContrast(float contrast)
{
  // Kontrastspannung darf maximal 80 sein
  analogWrite(CONN_DISP_VEE, 80 * constrain(contrast, 0, 1));
}

void DisplayController::SetIntensity(float intensity)
{
  // LED darf mit maximal 230 betrieben werden, da sie kein Vorwiderstand hat
  analogWrite(CONN_DISP_LED, 230 * constrain(intensity, 0, 1));
}

void DisplayController::NextError()
{
  // Nächsten Fehler in der Liste finden
  const byte showedError = ShowedError;
  Error* err = Errors.Find(
    [showedError](Error* err) -> bool { return err->Occurred && err->ID > showedError; });
  if (err == 0)
  {
    // Es gibt keine nachfolgenden Fehler
    if (ShowedError == -1
        || (err = Errors.Find([](Error* err) -> bool { return err->Occurred; })))
    {
      // Es wurde von Anfang an gesucht, also sind keine Fehler vorhanden
      ClearScreen();
      ErrorKey->Led.SetRatio(BiColorLED::RG_Green);
      ErrorKey->Led.SetPulsePerSecond(0);
      return;
    }
    else
    {
      // Es wurde ein nächster Fehler gefunden
      ShowedError = err->ID;
      ShowError();
    }
  }
}
void DisplayController::AcknowledgeError()
{
  // Abbrechen, wenn kein Fehler angezeigt wird
  if(ShowedError == -1)
    return;

  if(*IsKeyActive)
  {
    // Schlüsselschalter aktiv, also alle Fehler löschen
    Errors.ForEach([](Error* err) { err->Occurred = false; });
  }
  else
  {
    // Den angezeigten Fehler löschen
    const byte showedError = ShowedError;
    Errors.Find([showedError](Error* err) -> bool
      { return err->ID == showedError; })->Occurred = false;
  }
  NextError();
}
void DisplayController::ShowError()
{
  MessageTimeout.Stop();

  // Fehler zählen
  long errorCount = Errors.Count([](Error* err) -> bool
    { return err->Occurred; });

  // Position des anzuzeigenden Fehlers in dieser Menge bestimmen
  const byte showedError = ShowedError;
  long errorIndex = Errors.Count([showedError](Error* err) -> bool
    { return err->Occurred && err->ID <= showedError; });

  // Fehlertitel anzeigen
  Lcd.clear();
  Lcd.setCursor(0, 0);
  Lcd.print("F");
  Lcd.print(errorIndex);
  Lcd.print("/");
  Lcd.print(errorCount);
  Lcd.print(" ");

  // Fehlertext für eine bestimmte Zeit anzeigen
  Error* err = Errors.Find([showedError](Error* err) -> bool
    { return err->ID == showedError; });
  PrintTwoLine(err->Name, TEXT_INCORRECT);
  ErrorTimeout.Start();
}
void DisplayController::RestartErrorTimeout()
{
  ErrorTimeout.Start();
}
void DisplayController::ClearScreen()
{
  ErrorTimeout.Stop();
  MessageTimeout.Stop();
  ShowedError = -1;
  Lcd.clear();
}
void DisplayController::PrintTwoLine(const __FlashStringHelper* msg1, const __FlashStringHelper* msg2)
{
  if (msg1 != 0)
  {
    Lcd.print(msg1);
  }
  if (msg2 != 0)
  {
    Lcd.setCursor(0, 1);
    Lcd.print(msg2);
  }
}
