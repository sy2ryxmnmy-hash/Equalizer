#include "script.h"

#include <cstdio>

// ============================================================================
//  script.cpp
//  Hier kommen alle Scripts rein, die beim Initialisieren laufen sollen.
//  Einfach unten in RunInitScripts() weitere Aufrufe ergaenzen.
// ============================================================================

namespace atomic::script
{
    // ------------------------------------------------------------------
    // Beispiel-Script: hier eigene Init-Logik einbauen
    // ------------------------------------------------------------------
    static void InitScript_Example()
    {
        // TODO: eigenes Init-Script hier
    }

    void RunInitScripts()
    {
        InitScript_Example();
        // Weitere Init-Scripts hier aufrufen:
        // InitScript_MeinScript();
    }

    void RunShutdownScripts()
    {
        // Aufraeum-Scripts hier aufrufen
    }

    bool StartLoudmice(char* outError, int outErrorSize)
    {
        // Noch nicht implementiert -> Fehler zurueckgeben
        std::snprintf(outError, outErrorSize,
                      "Error: 'Start loudmice' ist noch nicht implementiert.");
        return false;
    }
}
