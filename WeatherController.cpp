#include "WeatherController.h"

WeatherController::WeatherController(NkkKey* weatherKey)
  : WeatherKey(weatherKey),
    DisplayDateTime(TIME_WEATHER_REQUESTDELAY),
    DisplayWeather(TIME_WEATHER_REQUESTDELAY + TIME_WEATHER_DISPLAYDELAY),
    DisplayAlarms(TIME_WEATHER_REQUESTDELAY + 2 * TIME_WEATHER_DISPLAYDELAY),
    RainAlarm(false), WindAlarm(false)
{
  
}

void WeatherController::Begin()
{
  // Led initialisieren
  WeatherKey->Led.SetRatio(BiColorLED::RG_Yellow);
  WeatherKey->Led.On();

  // Delegates verknüpfen
  WeatherKey->Btn.LongPressEvent = new Delegate<>(this, &WeatherController::SwitchFoyerLightOn);
  WeatherKey->Btn.ClickEvent = new Delegate<>(this, &WeatherController::RequestWeather);
  DisplayDateTime.TimeIsUpEvent = new Delegate<>(this, &WeatherController::ShowDateTime);
  DisplayWeather.TimeIsUpEvent = new Delegate<>(this, &WeatherController::ShowWeather);
  DisplayAlarms.TimeIsUpEvent = new Delegate<>(this, &WeatherController::ShowAlarms);
  SIMKNX128::AnyValueRecvEvent.Connect(this, &WeatherController::WeatherKnxObjectReceived);
}

void WeatherController::Update()
{
  DisplayDateTime.Update();
  DisplayWeather.Update();
  DisplayAlarms.Update();
}

void WeatherController::WeatherKnxObjectReceived(byte object, char* value)
{
  switch (object)
  {
  case KNX_WEATHER_DATE:
      Day = atoi(strtok(value, " "));
      Month = atoi(strtok(NULL, " "));
      Year = atoi(strtok(NULL, " "));
      break;

  case KNX_WEATHER_TIME:
      strtok(value, " ");
      Hour = atoi(strtok(NULL, " "));
      Minute = atoi(strtok(NULL, " "));
      break;

  case KNX_WEATHER_TEMP:
      Temperature = atof(strtok(value, " "));
      break;

  case KNX_WEATHER_TEMPMIN:
      TemperatureMin = atof(strtok(value, " "));
      break;

  case KNX_WEATHER_TEMPMAX:
      TemperatureMax = atof(strtok(value, " "));
      break;

  case KNX_WEATHER_WIND:
      Wind = atof(strtok(value, " "));
      break;
  
  case KNX_WEATHER_WINDALARM:
      if (SIMKNX128::ParseBool(value))
      {
          WindAlarm = true;
          WeatherKey->Led.SetRatio(BiColorLED::RG_Red);
          Global::Disp.ShowMessage(F("Windalarm - Bitte"), F("Jalousien öffnen"));
      }
      else
      {
          WindAlarm = false;
          if (!RainAlarm)
              WeatherKey->Led.SetRatio(BiColorLED::RG_Yellow);
      }
      break;
  
  case KNX_WEATHER_RAINALARM:
      if (SIMKNX128::ParseBool(value))
      {
          RainAlarm = true;
          WeatherKey->Led.SetRatio(BiColorLED::RG_Red);
          Global::Disp.ShowMessage(F("Regenalarm - Dach-"), F("fenster f\xE1hrt zu"));
      }
      else
      {
          RainAlarm = false;
          if (!WindAlarm)
              WeatherKey->Led.SetRatio(BiColorLED::RG_Yellow);
      }
      break;
    }
}

void WeatherController::RequestWeather()
{
  Global::Knx.SendBool(KNX_WEATHER_DATETIMEREQUEST, true);
  Global::Knx.SendBool(KNX_WEATHER_WEATHERREQUEST, true);
  DisplayDateTime.Start();
  DisplayWeather.Start();
  DisplayAlarms.Start();
}

void WeatherController::SwitchFoyerLightOn()
{
  Global::Knx.SendBool(KNX_WEATHER_FOYERLIGHTON, true);
  Global::Disp.ShowMessage(TEXT_LIGHTFOYER, TEXT_ON);
}

void WeatherController::ShowDateTime()
{
  char Line1[20], Line2[20];
  sprintf(Line1, "     %02i.%02i.%i", Day, Month, Year);
  sprintf(Line2, "       %02i:%02i", Hour, Minute);
  Global::Disp.ShowMessage(Line1, Line2, TIME_WEATHER_DISPLAYDELAY);
}

void WeatherController::ShowWeather()
{
  char Line1[20], Line2[20];
  sprintf(Line1, "TP %2i,%01i\xDF""C WI %2i,%01im/s",
    (int)Temperature, (int)((Temperature - (int)Temperature) * 10),
    (int)Wind, (int)((Wind - (int)Wind) * 10));
  sprintf(Line2, "LO %2i,%01i\xDF""C HI %2i,%01i\xDF""C",
    (int)TemperatureMin, (int)((TemperatureMin - (int)TemperatureMin) * 10),
    (int)TemperatureMax, (int)((TemperatureMax - (int)TemperatureMax) * 10));
  Global::Disp.ShowMessage(Line1, Line2, TIME_WEATHER_DISPLAYDELAY);
}

void WeatherController::ShowAlarms()
{
  char Line1[20], Line2[20];
  sprintf(Line1, "Windalarm: %s", WindAlarm ? "ja" : "nein");
  sprintf(Line1, "Regenalarm: %s", RainAlarm ? "ja" : "nein");
  Global::Disp.ShowMessage(Line1, Line2, TIME_WEATHER_DISPLAYDELAY);
}
