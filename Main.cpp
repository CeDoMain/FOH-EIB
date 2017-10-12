#include "Main.h"

Main::Main()
    // Hauptfunktionen (Audio, Video, ...) und Steuereinheiten (Fenster, Display, ...) konfigurieren
    // [Name](&Global::Key[y][x], [sendeObjekt], F("[Name max. 20 Zeichen]")),
  : Audio(&KEY_AUDIO, 10, F("Audio"), TIME_AUDIO_TIMEOUT),
    Video(&KEY_VIDEO, 20, F("Video")),
    Licht(&KEY_LIGHT, 50, F("Licht")),
    NutzungF1(&KEY_USEF1, 60, F("Nutzung Forum 1")),
    NutzungF2(&KEY_USEF2, 62, F("Nutzung Forum 2")),
    DMK(&KEY_DMK, 18, F("Dolmetscherkabinen")),
    Livestream(&KEY_LIVESTREAM, 40, F("Livestream")),
    SeWo(&KEY_SEWO, 42, F("Seniorenwohnanlage")),
    Pflegeheim(&KEY_PFLEGEHEIM, 44, F("Pflegeheim")),
    
    // Hauptfunktion Beamer
    Beamer(&KEY_BEAMER),
    // Fenster- und Jalousiesteuerung
    WinControl(&KEY_WINDOWUP, &KEY_WINDOWDOWN),
    // Helligkeit der Tasten und Displaybeleuchtung einstellen
    IntControl(&KEY_INTENSITY),
    // Langer Druck auf den Taster: Licht im Foyer einschalten, Kurzer Druck: Wetter und Uhrzeit abfrage
    WeatherControl(&KEY_WEATHER)
{
  // Schaltbedingungen (NutzungF1, Regen, Audio eingeschaltet, ...) konfigurieren
  // Hier kann man Programmcode reinschreiben, der ausgeführt wird,
  // um herauszubekommen ob eine Funktion geschaltet werden darf
  // Rückmeldung true: darf geschaltet werden (andernfalls nicht)
  // Meldungen auf dem Display bei Sperre sind erlaubt und erwünscht
  MasterAudio = new Delegate<bool>([this]() -> bool
  {
    if (Audio.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Audio n. aktiv")); return false; }
  });
  MasterVideo = new Delegate<bool>([this]() -> bool
  {
    if (Video.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Video n. aktiv")); return false; }
  });
  MasterNutzungF1 = new Delegate<bool>([this]() -> bool
  {
    if (NutzungF1.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Nutzung F1 n. aktiv")); return false; }
  });
  MasterNutzungF2 = new Delegate<bool>([this]() -> bool
  {
    if (NutzungF2.IsOn()) { return true; }
    else { Global::Disp.ShowMessage(TEXT_NOT_AVAILABLE, F("Nutzung F2 n. aktiv")); return false; }
  });

  // Rückmeldungen konfigurieren
  // [Name].AddRecvObj[empfangsObjekt], F("[Name max. 20 Zeichen]"));
  Audio.AddRecvObj(11, F("F32 Audio Verst. 1"));
  Audio.AddRecvObj(12, F("F33 Audio Verst. 2"));
  Audio.AddRecvObj(13, F("F34 Audio Verst. 3"));
  Audio.AddRecvObj(14, F("F35 Audio Verst. 4"));
  Audio.AddRecvObj(15, F("F36 Audio Verst. 5"));
  Audio.AddRecvObj(16, F("F37 Audio FOH"));
  Audio.AddRecvObj(17, F("F38 Audio MixRack"));

  Video.AddRecvObj(21, F("F?? Video FOH"));

  Licht.AddRecvObj(51, F("F?? Licht FOH"));

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
  WinControl.AddWindow(90, 91, 92, 93, F("F1 Fenster Dach"), &KEY_WINDOWF1ROOF, 0);
  WinControl.AddWindow(95, 96, 97, 98, F("F1 Fenster Seite"), &KEY_WINDOWF1SIDE, 0);
  WinControl.AddWindow(105, 106, 107, 108, F("F2 Fenster"), &KEY_WINDOWF2, MasterNutzungF2);

  // Jalousiesteuerung konfigurieren
  // WinControl.AddJalousie([Fahren-Objekt], [Step-Objekt],
  //   [Rückmeldung-Oben-Objekt], [Rückmeldung-Unten-Objekt]
  //   F("[Name max. 20 Zeichen]"), &Global::Key[y][x],
  //   [Aktivierungsbedingung => 0 oder z.B. &MasterNutzungF1]});
  WinControl.AddJalousie(70, 71, 72, 73, F("F1 Jalousie oben"), &KEY_JALOUSIEF1TOP, 0);
  WinControl.AddJalousie(75, 76, 77, 78, F("F1 Jalousie unten"), &KEY_JALOUSIEF1BTN, 0);
  WinControl.AddJalousie(80, 81, 82, 83, F("F1 Altar Jalousie"), &KEY_JALOUSIEF1ALT, 0);
  WinControl.AddJalousie(85, 86, 87, 88, F("F1 Altar Rollo"), &KEY_JALOUSIEF1ROL, 0);
  WinControl.AddJalousie(100, 101, 102, 103, F("F2 Jalousie"), &KEY_JALOUSIEF2, MasterNutzungF2);

  // Einschalt-Abhängigkeiten zwischen Hauptfunktionen
  // [Name der Slavefunktion].CanActivate = &[Aktivierungsbedingung => Zeile entfernen oder z.B. MasterNutzungF1];
  NutzungF2.CanActivate = MasterNutzungF1;
  DMK.CanActivate = MasterAudio;
  Livestream.CanActivate = MasterVideo;
  SeWo.CanActivate = MasterVideo;
  Pflegeheim.CanActivate = MasterVideo;
  Beamer.CanActivate = MasterVideo;
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
