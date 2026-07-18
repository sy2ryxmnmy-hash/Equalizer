#include "gui.h"
#include "script.h"

#include <windows.h>
#include <d3d11.h>
#include <atomic>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace atomic::gui
{
    // ------------------------------------------------------------------
    // D3D11-State
    // ------------------------------------------------------------------
    static ID3D11Device*           g_Device        = nullptr;
    static ID3D11DeviceContext*    g_Context       = nullptr;
    static IDXGISwapChain*         g_SwapChain     = nullptr;
    static ID3D11RenderTargetView* g_RenderTarget  = nullptr;
    static std::atomic<bool>       g_ShutdownFlag  = false;

    static void CreateRenderTarget()
    {
        ID3D11Texture2D* backBuffer = nullptr;
        g_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        g_Device->CreateRenderTargetView(backBuffer, nullptr, &g_RenderTarget);
        backBuffer->Release();
    }

    static void CleanupRenderTarget()
    {
        if (g_RenderTarget) { g_RenderTarget->Release(); g_RenderTarget = nullptr; }
    }

    static bool CreateDeviceD3D(HWND hwnd)
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount        = 2;
        sd.BufferDesc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow       = hwnd;
        sd.SampleDesc.Count   = 1;
        sd.Windowed           = TRUE;
        sd.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL level;
        const D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
        if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                                 levels, 2, D3D11_SDK_VERSION, &sd,
                                                 &g_SwapChain, &g_Device, &level, &g_Context)))
            return false;

        CreateRenderTarget();
        return true;
    }

    static void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_SwapChain) { g_SwapChain->Release(); g_SwapChain = nullptr; }
        if (g_Context)   { g_Context->Release();   g_Context = nullptr; }
        if (g_Device)    { g_Device->Release();    g_Device = nullptr; }
    }

    static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SIZE:
            if (g_Device && wParam != SIZE_MINIMIZED)
            {
                CleanupRenderTarget();
                g_SwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam),
                                           DXGI_FORMAT_UNKNOWN, 0);
                CreateRenderTarget();
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    // ------------------------------------------------------------------
    // Haupt-UI (pro Frame)
    // ------------------------------------------------------------------
    static void DrawUi()
    {
        static char errorMsg[256] = "";

        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(360, 160), ImGuiCond_FirstUseEver);
        ImGui::Begin("Atomic EQ Plugin");

        if (ImGui::Button("Start loudmice"))
        {
            atomic::script::StartLoudmice(errorMsg, sizeof(errorMsg));
        }

        if (errorMsg[0] != '\0')
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s", errorMsg);

        ImGui::End();
    }

    // ------------------------------------------------------------------
    // GUI-Thread
    // ------------------------------------------------------------------
    void RunGuiThread(void* hModule)
    {
        WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0, 0,
                           (HINSTANCE)hModule, nullptr, nullptr, nullptr, nullptr,
                           L"AtomicEqPluginWnd", nullptr };
        RegisterClassExW(&wc);
        HWND hwnd = CreateWindowW(wc.lpszClassName, L"Atomic EQ Plugin",
                                  WS_OVERLAPPEDWINDOW, 100, 100, 500, 320,
                                  nullptr, nullptr, wc.hInstance, nullptr);

        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            DestroyWindow(hwnd);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return;
        }

        ShowWindow(hwnd, SW_SHOWDEFAULT);
        UpdateWindow(hwnd);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(g_Device, g_Context);

        bool running = true;
        while (running && !g_ShutdownFlag)
        {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    running = false;
            }
            if (!running)
                break;

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            DrawUi();

            ImGui::Render();
            const float clear[4] = { 0.06f, 0.06f, 0.06f, 1.0f };
            g_Context->OMSetRenderTargets(1, &g_RenderTarget, nullptr);
            g_Context->ClearRenderTargetView(g_RenderTarget, clear);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            g_SwapChain->Present(1, 0);
        }

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        DestroyWindow(hwnd);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
    }

    void RequestShutdown()
    {
        g_ShutdownFlag = true;
    }
}
