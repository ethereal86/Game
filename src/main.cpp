#include <pch.h>

#include "Platform/Window.h"
#include "Renderer/Renderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    Window window(1200, 900, L"Game");
    RHI rhi(window.Get(), 1200, 900);
    Renderer renderer(rhi);

    window.Show();

    bool running = true;
    while (running) 
    {
        if (!window.ProcessMessages())
            running = false;

        renderer.BeginFrame();
        renderer.Render();
        renderer.EndFrame();
    }
    rhi.Flush();

    return 0;
}