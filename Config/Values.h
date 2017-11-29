#ifndef VALUES_H
#define VALUES_H

#define TIME_BEAMER_TIMEOUT           300000    // Zeit in ms, nach der der Beamerschaltvorgang abgeschlossen sein muss
#define TIME_BEAMER_GETSTATE          2000      // Intervall in ms, in dem ein Status vom Beamer abgefragt wird
#define TIME_DEFAULT_TIMEOUT          20000     // Zeit in ms, nach der die Hauptfunktionen geschaltet haben müssen
#define TIME_AUDIO_TIMEOUT            70000     // Zeit in ms, nach der die Audiohauptfunktion geschaltet haben muss
#define TIME_LIGHT_TIMEOUT            70000     // Zeit in ms, nach der die Lichthauptfunktion geschaltet haben muss

#define TIME_DISP_MSGTIMEOUT          3000      // Anzeigedauer in ms für eine Mitteilung
#define TIME_DISP_ERRTIMEOUT          5000      // Anzeigedauer in ms für eine Fehlermeldung

#define LGHT_DIMM_CONTRAST            0.4       // Standardkontrast
#define LGHT_DIMM_ACTIVEINTENSITY     10000     // Standardhelligkeit bei Aktivierung [0;10000]
#define LGHT_DIMM_DIMMEDINTENSITY     100       // Standardhelligkeit nach Dimmen der Anzeigen [0;10000]
#define TIME_DIMM_TIMEOUT             15000     // Zeit in ms nach der die Anzeigen gedimmt werden
#define TIME_DIMM_DIMMTIME            2000      // Zeit in ms in der die Anzeigen gedimmt werden
#define TIME_DIMM_WAKEUPTIME          200       // Zeit in ms in der die Anzeigen aktiviert werden

#define TIME_WEATHER_REQUESTDELAY     1000      // Zeit in ms, zwischen Anfrage und Anzeige der Wetter und Zeitinformationen
#define TIME_WEATHER_DISPLAYDELAY     5000      // Zeit in ms, die Wetter und Zeit angezeigt werden

#define TIME_WINDOW_DESELECT          5000      // Zeit in ms, nach der ein gewähltes Fenster oder eine Jalousie wieder abgewählt wird

#define TIME_PULSERATE                0.75      // Pulsiergeschwindigkeit in P/s aller blinkenden Taster

#endif
