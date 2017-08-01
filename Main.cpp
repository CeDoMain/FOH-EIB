#include "Main.h"

Main::Main()
    // Hauptfunktionen (Audio, Video, ...) und Steuereinheiten (Fenster, Display, ...) konfigurieren
    // [Name](&Global::Key[y][x], [sendeObjekt], F("[Name max. 20 Zeichen]")),
  : Audio(&Global::Key[1][4], 10, F("Audio")),
    Video(&Global::Key[1][3], 20, F("Video")),
    Licht(&Global::Key[1][2], 50, F("Licht")),
    NutzungF1(&Global::Key[1][1], 60, F("Nutzung Forum 1")),
    NutzungF2(&Global::Key[1][0], 62, F("Nutzung Forum 2")),
    DMK(&Global::Key[2][4], 18, F("Dolmetscherkabinen")),
    Livestream(&Global::Key[2][3], 40, F("Livestream")),
    SeWo(&Global::Key[2][2], 42, F("Seniorenwohnanlage")),
    Pflegeheim(&Global::Key[2][1], 44, F("Pflegeheim")),
    Beamer(&Global::Key[2][0]),
    // Weitere Funktionen konfigurieren
    // Fenstersteuerung(Taster für "offen/oben", Taster für "schließen/unten")
    WinControl(&Global::Key[3][4], &Global::Key[4][4]),
    // Helligkeit der Tasten und Displaybeleuchtung einstellen (bei Schlüsselaktivierung: Kontrast)
    IntControl(&Global::Key[0][3]),
    // Langer Druck auf den Taster: Licht im Foyer einschalten, Kurzer Druck: Wetter und Uhrzeit abfrage
    WeatherControl(&Global::Key[0][2])
{
  // Schaltbedingungen (NutzungF1, Regen, Audio eingeschaltet, ...) konfigurieren
  // Hier kann man Programmcode reinschreiben, der ausgeführt wird,
  // um herauszubekommen ob eine Funktion geschaltet werden darf
  // Rückmeldung true: darf geschaltet werden (andernfalls nicht)
  // Meldungen auf dem Display bei Sperre sind erlaubt und erwünscht
  MasterAudio = Delegate<bool>([this]() -> bool
  {
    if (Audio.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Audio n. aktiv")); return false; }
  });
  MasterVideo = Delegate<bool>([this]() -> bool
  {
    if (Video.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Video n. aktiv")); return false; }
  });
  MasterNutzungF1 = Delegate<bool>([this]() -> bool
  {
    if (NutzungF1.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Nutzung F1 n. aktiv")); return false; }
  });
  MasterNutzungF2 = Delegate<bool>([this]() -> bool
  {
    if (NutzungF2.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Nutzung F2 n. aktiv")); return false; }
  });
  MasterNutzungF1AndRain = Delegate<bool>([this]() -> bool
  {
    if (Global::IsRaining) { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("es regnet")); return false; }
    else if (!NutzungF1.IsOn()) { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Nutzung F1 n. aktiv")); return false; }
    else { return true; }
    return false;
  });
  // Rückmeldungen konfigurieren
  // [Name].AddRecvObj[empfangsObjekt], F("[Name max. 20 Zeichen]"));
  Audio.AddRecvObj(11, F("Mischpult"));
  Audio.AddRecvObj(12, F("Siderack"));
  Audio.AddRecvObj(13, F("Verst\xE1rker A"));
  Audio.AddRecvObj(14, F("Verst\xE1rker B"));
  Audio.AddRecvObj(15, F("Verst\xE1rker C"));

  Video.AddRecvObj(21, F("Videohub"));
  Video.AddRecvObj(22, F("Videomischer"));
  Video.AddRecvObj(23, F("Bildschirme"));
  Video.AddRecvObj(24, F("Kameras"));

  Licht.AddRecvObj(51, F("Lichtmischpult"));
  Licht.AddRecvObj(52, F("Dimmer A"));
  Licht.AddRecvObj(53, F("Dimmer B"));
  Licht.AddRecvObj(54, F("Dimmer C"));
  Licht.AddRecvObj(55, F("Dimmer D"));

  NutzungF1.AddRecvObj(61, 0);
  NutzungF2.AddRecvObj(63, 0);
  DMK.AddRecvObj(19, 0);
  Livestream.AddRecvObj(41, 0);
  SeWo.AddRecvObj(43, 0);
  Pflegeheim.AddRecvObj(45, 0);

  // Fenstersteuerung konfigurieren
  // WinControl.AddWindow([Fahren-Objekt], [Step-Objekt],
  //   [Rückmeldung-Offen-Objekt], [Rückmeldung-Geschlossen-Objekt]
  //   F("[Name max. 20 Zeichen]"), &Global::Key[y][x],
  //   [Aktivierungsbedingung => 0 oder z.B. &MasterNutzungF1]});
  WinControl.AddWindow(90, 91, 92, 93, F("F1 Fenster Dach"), &Global::Key[3][1], 0);
  WinControl.AddWindow(95, 96, 97, 98, F("F1 Fenster Seite"), &Global::Key[4][1], 0);
  WinControl.AddWindow(105, 106, 107, 108, F("F2 Fenster"), &Global::Key[3][0], 0);

  // Jalousiesteuerung konfigurieren
  // WinControl.AddJalousie([Fahren-Objekt], [Step-Objekt],
  //   [Rückmeldung-Oben-Objekt], [Rückmeldung-Unten-Objekt]
  //   F("[Name max. 20 Zeichen]"), &Global::Key[y][x],
  //   [Aktivierungsbedingung => 0 oder z.B. &MasterNutzungF1]});
  WinControl.AddJalousie(70, 71, 72, 73, F("F1 Jalousie oben"), &Global::Key[3][3], 0);
  WinControl.AddJalousie(75, 76, 77, 78, F("F1 Jalousie unten"), &Global::Key[4][3], 0);
  WinControl.AddJalousie(80, 81, 82, 83, F("F1 Altar Jalousie"), &Global::Key[4][2], 0);
  WinControl.AddJalousie(85, 86, 87, 88, F("F1 Altar Rollo"), &Global::Key[3][2], 0);
  WinControl.AddJalousie(100, 101, 102, 103, F("F2 Jalousie"), &Global::Key[4][0], 0);

  // Schalt-Abhängigkeiten zwischen Hauptfunktionen
  // [Name der Slavefunktion].CanActivate = &[Aktivierungsbedingung => Zeile entfernen oder z.B. MasterNutzungF1];
  NutzungF2.CanActivate = &MasterNutzungF1;
  DMK.CanActivate = &MasterAudio;
  Livestream.CanActivate = &MasterVideo;
  SeWo.CanActivate = &MasterVideo;
  Pflegeheim.CanActivate = &MasterVideo;
  Beamer.CanActivate = &MasterVideo;
}

void Main::Begin()
{
  WinControl.Begin();
  IntControl.Begin();
  WeatherControl.Begin();

  // Ausschaltabhängigkeiten zwischen Hauptfunktionen (Masterfunktion wird ausgeschaltet => Slavefunktion wird auch ausgeschaltet)
  // [Name der Masterfunktion].SwitchingOffEvent.Connect(&[Name der Slavefunktion],
  //   [Typ der Slavefunkton => MainFunction oder BeamerFunktion]::SwitchOff);
  Video.SwitchingOffEvent.Connect(&Beamer, &BeamerFunction::SwitchOff);
  Video.SwitchingOffEvent.Connect(&Livestream, &MainFunction::SwitchOff);
  Video.SwitchingOffEvent.Connect(&SeWo, &MainFunction::SwitchOff);
  Video.SwitchingOffEvent.Connect(&Pflegeheim, &MainFunction::SwitchOff);
  Audio.SwitchingOffEvent.Connect(&DMK, &MainFunction::SwitchOff);
  NutzungF1.SwitchingOffEvent.Connect(&NutzungF2, &MainFunction::SwitchOff);

  // Hauptfunktionen initialisieren
  Audio.Begin();
  Video.Begin();
  Licht.Begin();
  DMK.Begin();
  Livestream.Begin();
  SeWo.Begin();
  Pflegeheim.Begin();
  NutzungF1.Begin();
  NutzungF2.Begin();
  Beamer.Begin();
}

void Main::Update()
{
  IntControl.Update();
  WinControl.Update();
  WeatherControl.Update();

  Audio.Update();
  Video.Update();
  Licht.Update();
  DMK.Update();
  Livestream.Update();
  SeWo.Update();
  Pflegeheim.Update();
  NutzungF1.Update();
  NutzungF2.Update();
  Beamer.Update();
}
