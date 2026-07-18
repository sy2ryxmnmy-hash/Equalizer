#pragma once

// GUI des Plugins (ImGui + Win32 + DirectX 11).
namespace atomic::gui
{
    // Blockiert, bis das Fenster geschlossen wird. Laeuft im eigenen Thread.
    void RunGuiThread(void* hModule);

    // Signalisiert dem GUI-Thread, sich zu beenden.
    void RequestShutdown();
}
