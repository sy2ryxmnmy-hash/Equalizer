#pragma once

// Scripts, die beim Initialisieren des Plugins laufen.
// Neue Scripts in script.cpp registrieren / einfuegen.
namespace atomic::script
{
    // Wird einmal beim Laden der DLL (vor dem GUI-Start) aufgerufen.
    void RunInitScripts();

    // Wird beim Entladen der DLL aufgerufen.
    void RunShutdownScripts();

    // Aktion hinter dem "Start loudmice"-Button.
    // Gibt false zurueck und befuellt outError, wenn etwas schiefgeht.
    bool StartLoudmice(char* outError, int outErrorSize);
}
