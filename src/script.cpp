#include "script.h"

#include <cstdio>

namespace atomic::script
{
    static void InitScript_Example()
    {
        // NOTE: Init-Script bzw bd
    }

    void RunInitScripts()
    {
        InitScript_Example();
        // hier initializen
    }

    void RunShutdownScripts()
    {
        // shutdown callen
    }

    bool StartLoudmice(char* outError, int outErrorSize)
    {
        // Noch nicht implementiert -> Fehler zurueckgeben
        std::snprintf(outError, outErrorSize,
                      "Error: 'Start loudmice' ist noch nicht implemented.");
        return false;
    }
}
