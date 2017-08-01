#ifndef TEXTS_H
#define TEXTS_H

#include "Arduino.h"

// Hier stehen alle mehrfachverwendeten Texte
// maximal 20 Zeichen pro Text und in F("") eingeschlossen
// Umlaute im Displaytext:
//  ä = \xE1
//  ö = \xEF
//  ü = \xF5
//  Umlaut-Großbuchstaben gibt es nicht

#define TEXT_KEY                    F("Schl\xF5ssel")
#define TEXT_NOT_ACTIVE             F("nicht aktiv")
#define TEXT_ACTIVATED              F("aktiviert")
#define TEXT_DEACTIVATED            F("deaktiviert")
#define TEXT_ALREADY_SWITCHING_1    F("Schaltvorgang")
#define TEXT_ALREADY_SWITCHING_2    F("l\u00F5uft bereits")

#define TEXT_AUDIO                  F("Audio")
#define TEXT_VIDEO                  F("Video")
#define TEXT_LIGHT                  F("Licht")
#define TEXT_USE_F1                 F("Nutzung F1")
#define TEXT_USE_F2                 F("Nutzung F2")
#define TEXT_BEAMER                 F("Beamer")

#define TEXT_SWITCHING_ON           F("wird eingeschaltet")
#define TEXT_SWITCHING_OFF          F("wird ausgeschaltet")
#define TEXT_ON                     F("eingeschaltet")
#define TEXT_OFF                    F("ausgeschaltet")
#define TEXT_INCORRECT              F("fehlerhaft")

#define TEXT_BEAMER_NOTIFY          F("Beamer meldet")
#define TEXT_BEAMER_BOOTING         F("Einschalten")
#define TEXT_BEAMER_COUNTDOWN       F("Zur\xF5""ckz\xE1hlen")
#define TEXT_BEAMER_COOLDOWN        F("Abk\xF5hlen")

#define TEXT_INTENSITY              F("Helligkeit: ")
#define TEXT_CONTRAST               F("Kontrast: ")
#define TEXT_NOT_AVAILABLE          F("nicht verf\xF5gbar")

#define TEXT_LIGHTFOYER             F("Licht im Foyer")
#define TEXT_SELECTED               F("ausgew\xE1hlt")
#define TEXT_NOSELECTION            F("Keine Auswahl")
#define TEXT_STEPOPEN               F("Schritt auf")
#define TEXT_STEPCLOSE              F("Schritt zu")
#define TEXT_DRIVEOPEN              F("f\xE1hrt auf")
#define TEXT_DRIVECLOSE             F("f\xE1hrt zu")

#endif
