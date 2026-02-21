#include "Window.h"

#include <stdexcept>

Window::Window(int width, int height, const wchar_t* title) {
    m_hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = m_hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wc.lpszClassName = s_className;

    if (!RegisterClass(&wc))
        throw std::runtime_error("Failed to register window class");

    m_hWnd = CreateWindowEx(
        0, s_className, title, 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWnd)
        throw std::runtime_error("Failed to create window");

    ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window() {
    UnregisterClass(s_className, m_hInstance);
    DestroyWindow(m_hWnd);
}

bool Window::ProcessMessages() const {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}