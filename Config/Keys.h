#ifndef KEYS_H
#define KEYS_H

// Global::Key[y][x]
// y => Platinennummer
// x => Tasternummer (links=4, rechts=0)

#define KEY_ERROR         Global::Key[0][4]     // Taster um Fehlermeldungen anzuzeigen und zu löschen (ohne schlüssel: eine löschen / mit schlüssel: alle löschen)
#define KEY_INTENSITY     Global::Key[0][3]     // Taster um die Helligkeit des Displays und der Taster einzustellen
#define KEY_WEATHER       Global::Key[0][2]     // Taster um Uhrzeit und Wetter abzufragen, bzw. bei langem Druck das Foyerlicht einzuschalten
#define KEY_FOHLIGHT      Global::Key[0][1]     // Taster um Licht unter dem FOH zu schalten (langer druck)
#define KEY_INFO          Global::Key[0][0]     // Taster um Informationen über den Zustand der Software abzufragen

#define KEY_AUDIO         Global::Key[1][4]     // Taster für die Audiohauptfunktion
#define KEY_VIDEO         Global::Key[1][3]     // Taster für die Videohauptfunktion
#define KEY_LIGHT         Global::Key[1][2]     // Taster für die Lichthauptfunktion
#define KEY_USEF1         Global::Key[1][1]     // Taster für die Nutzung von Forum 1
#define KEY_USEF2         Global::Key[1][0]     // Taster für die Nutzung von Forum 2

#define KEY_DMK           Global::Key[2][4]     // Taster für die Dolmetscherkabine
#define KEY_LIVESTREAM    Global::Key[2][3]     // Taster für den Livestream
#define KEY_SEWO          Global::Key[2][2]     // Taster für die Übertragung zur SeWo
#define KEY_PFLEGEHEIM    Global::Key[2][1]     // Taster für die Übertragung ins Pflegeheim
#define KEY_BEAMER        Global::Key[2][0]     // Taster für die Beamerhauptfunktion

#define KEY_WINDOWUP      Global::Key[3][4]     // Taster um Fenster und Jalousien zu öffnen
#define KEY_JALOUSIEF1TOP Global::Key[3][3]     // Taster um die obere Jalousie in Forum 1 zu steuern
#define KEY_JALOUSIEF1ROL Global::Key[3][2]     // Taster um das Rollo am Altar zu steuern
#define KEY_WINDOWF1ROOF  Global::Key[3][1]     // Taster um die Dachfenster in Forum 1 zu steuern
#define KEY_JALOUSIEF2    Global::Key[3][0]     // Tatser um die Jalousien in Forum 2 zu steuern

#define KEY_WINDOWDOWN    Global::Key[4][4]     // Taster um die Fenster und Jalousien zu schließen
#define KEY_JALOUSIEF1BTN Global::Key[4][3]     // Taster um die untere Jalousie in Forum 1 zu steuern
#define KEY_JALOUSIEF1ALT Global::Key[4][2]     // Taster um die Jalousie am Altar zu steuern
#define KEY_WINDOWF1SIDE  Global::Key[4][1]     // Taster um die Seitenfenster in FOrum 1 zu steuern
#define KEY_WINDOWF2      Global::Key[4][0]     // Taster um die Fenster in Forum 2 zu steuern

#endif
