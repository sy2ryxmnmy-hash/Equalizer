#include "script.h"

#include <cstdio>

namespace atomic::script
{
    static void InitScript_Example()
    {
        // NOTE: here Initialize Script
    }

    void RunInitScripts()
    {
        InitScript_Example();
        // the initialize
    }

    void RunShutdownScripts()
    {
        // calling the shutdown
    }

    bool StartLoudmice(char* outError, int outErrorSize)
    {
        // Fake error
        std::snprintf(outError, outErrorSize,
                      "Error: 'Start loudmice' ist noch nicht implemented.");
        return false;
    }
}
