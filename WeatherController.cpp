#include "WeatherController.h"

WeatherController::WeatherController(NkkKey* weatherKey)
  : WeatherKey(weatherKey),
    SendLightFoyer(KNX_WEATHER_FOYERLIGHTON),
    SendDateTimeRequest(KNX_WEATHER_DATETIMEREQUEST),
    SendWeatherRequest(KNX_WEATHER_WEATHERREQUEST),
    RecvDate(KNX_WEATHER_DATE),
    RecvTime(KNX_WEATHER_TIME),
    RecvTemperature(KNX_WEATHER_TEMP),
    RecvTemperatureMin(KNX_WEATHER_TEMPMIN),
    RecvTemperatureMax(KNX_WEATHER_TEMPMAX),
    RecvWind(KNX_WEATHER_WIND),
    DisplayDateTime(TIME_WEATHER_REQUESTDELAY),
    DisplayWeather(TIME_WEATHER_REQUESTDELAY + TIME_WEATHER_DISPLAYDELAY)
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
  SIMKNX128::AnyValueRecvEvent.Connect(this, &WeatherController::WeatherKnxObjectReceived);
}
void WeatherController::Update()
{
  DisplayDateTime.Update();
  DisplayWeather.Update();
}
void WeatherController::WeatherKnxObjectReceived(byte object, char* value)
{
  if (object == RecvDate)
  {
    Day = atoi(strtok(value, " "));
    Month = atoi(strtok(NULL, " "));
    Year = atoi(strtok(NULL, " "));
  }
  else if (object == RecvTime)
  {
    strtok(value, " ");
    Hour = atoi(strtok(NULL, " "));
    Minute = atoi(strtok(NULL, " "));
  }
  else if (object == RecvTemperature)
  {
    Temperature = atof(strtok(value, " "));
  }
  else if (object == RecvTemperatureMin)
  {
    TemperatureMin = atof(strtok(value, " "));
  }
  else if (object == RecvTemperatureMax)
  {
    TemperatureMax = atof(strtok(value, " "));
  }
  else if (object == RecvWind)
  {
    Wind = atof(strtok(value, " "));
  }
}
void WeatherController::RequestWeather()
{
  Global::Knx.SendBool(SendDateTimeRequest, true);
  Global::Knx.SendBool(SendWeatherRequest, true);
  DisplayDateTime.Start();
  DisplayWeather.Start();
}
void WeatherController::SwitchFoyerLightOn()
{
  Global::Knx.SendBool(SendLightFoyer, true);
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
