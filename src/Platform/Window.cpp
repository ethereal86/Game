#include <pch.h>
#include "Window.h"

#include "Core/Assert.h"

Window::Window(int width, int height, const wchar_t* title)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = WindowClassName;

    WIN32_CHECK(RegisterClassEx(&wc), "Failed to register window class");

    RECT rect = {0, 0, width, height};
    constexpr DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    AdjustWindowRect(&rect, style, FALSE);

    m_hWnd = CreateWindowEx(
        0,
        WindowClassName,
        title,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    WIN32_CHECK(m_hWnd != nullptr, "Failed to create window");
}

Window::~Window()
{
    if (m_hWnd)
        DestroyWindow(m_hWnd);
}

bool Window::ProcessMessages() const
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
    {
        if (msg.message == WM_QUIT) 
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

void Window::Show() const
{
    ShowWindow(m_hWnd, SW_SHOW);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) 
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }
        
    return DefWindowProc(hWnd, msg, wParam, lParam);
}