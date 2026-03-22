#include <pch.h>

#include "Platform/Window.h"
#include "Renderer/Renderer.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, L"Game");
    RHI rhi(window.Get(), window.GetWidth(), window.GetHeight());
    Renderer renderer(rhi);

    window.Show();

    bool running = true;
    while (running) 
    {
        if (!window.ProcessMessages())
            running = false;

        renderer.Render();
    }
    rhi.Flush();

    return 0;
}