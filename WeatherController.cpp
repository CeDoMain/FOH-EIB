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

    // Taster mit Funktionen verknüpfen
    WeatherKey->Btn.LongPressEvent = new Delegate<>(this, &WeatherController::SwitchFoyerLightOn);
    WeatherKey->Btn.ClickEvent = new Delegate<>(this, &WeatherController::RequestWeather);

    // Timer verknüpfen
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
    // Rückmeldungen von Wetterstation verarbeiten und speichern
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
        Temperature = atof(strtok(value, " ")) * 10;
        break;

    case KNX_WEATHER_TEMPMIN:
        TemperatureMin = atof(strtok(value, " ")) * 10;
        break;

    case KNX_WEATHER_TEMPMAX:
        TemperatureMax = atof(strtok(value, " ")) * 10;
        break;

    case KNX_WEATHER_WIND:
        Wind = atof(strtok(value, " ")) * 10;
        break;
    
    case KNX_WEATHER_WINDALARM:
        if (SIMKNX128::ParseBool(value))
        {
            WindAlarm = true;
            WeatherKey->Led.SetRatio(BiColorLED::RG_Red);
            Global::Disp.ShowMessage(F("Windalarm - Bitte"), F("Jalousien \xEFffnen"));
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
    // Rückmeldung von Wetterstation anfordern
    Global::Knx.SendBool(KNX_WEATHER_DATETIMEREQUEST, true);
    Global::Knx.SendBool(KNX_WEATHER_WEATHERREQUEST, true);
    DisplayDateTime.Start();
    DisplayWeather.Start();
    DisplayAlarms.Start();
}

void WeatherController::SwitchFoyerLightOn()
{
    // Licht im Forum Foyer einschalten (aus Sicherheitsgründen kein Ausschalten)
    Global::Knx.SendBool(KNX_WEATHER_FOYERLIGHTON, true);
    Global::Disp.ShowMessage(TEXT_LIGHTFOYER, TEXT_ON);
}

void WeatherController::ShowDateTime()
{
    // Datum und Uhrzeit anzeigen
    char Line1[21], Line2[21];
    sprintf(Line1, "     %02i.%02i.%i", Day, Month, Year);
    sprintf(Line2, "       %02i:%02i", Hour, Minute);
    Global::Disp.ShowMessage(Line1, Line2, TIME_WEATHER_DISPLAYDELAY);
}

void WeatherController::ShowWeather()
{
    // Wetterdaten anzeigen
    char Line1[21], Line2[21];
    sprintf(Line1, "T %3i.%01i\xDF""C WI %2i.%01im/s",
        Temperature / 10, abs(Temperature % 10),
        Wind / 10, abs(Wind % 10));
    sprintf(Line2, "LO %3i.%01i\xDF""C HI %2i.%01i\xDF""C",
        TemperatureMin / 10, abs(TemperatureMin % 10),
        TemperatureMax / 10, abs(TemperatureMax % 10));
    Global::Disp.ShowMessage(Line1, Line2, TIME_WEATHER_DISPLAYDELAY);
}

void WeatherController::ShowAlarms()
{
    // Wetteralarme anzeigen
    char Line1[21], Line2[21];
    sprintf(Line1, "Windalarm: %s", WindAlarm ? "ja" : "nein");
    sprintf(Line2, "Regenalarm: %s", RainAlarm ? "ja" : "nein");
    Global::Disp.ShowMessage(Line1, Line2, TIME_WEATHER_DISPLAYDELAY);
}
