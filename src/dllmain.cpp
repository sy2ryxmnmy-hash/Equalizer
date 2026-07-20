#include <windows.h>

#include "gui.h"
#include "script.h"

static HANDLE g_GuiThread = nullptr;

static DWORD WINAPI GuiThreadEntry(LPVOID hModule)
{
    atomic::gui::RunGuiThread(hModule);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        atomic::script::RunInitScripts();
        g_GuiThread = CreateThread(nullptr, 0, GuiThreadEntry, hModule, 0, nullptr);
        break;

    case DLL_PROCESS_DETACH:
        atomic::gui::RequestShutdown();
        if (g_GuiThread)
        {
            CloseHandle(g_GuiThread);
            g_GuiThread = nullptr;
        }
        atomic::script::RunShutdownScripts();
        break;
    }
    return TRUE;
}
