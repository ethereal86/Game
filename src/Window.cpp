#include <pch.h>
#include "Window.h"

Window::Window(HINSTANCE hInstance, int width, int height, const char* title)
    : m_hInstance(hInstance)
{
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wcex.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wcex);

    DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

    RECT rect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    AdjustWindowRectEx(&rect, style, FALSE, 0);

    m_hWnd = CreateWindowEx(
        0, CLASS_NAME, title,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, hInstance,
        this
    );

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}

Window::~Window() {
    if (m_hWnd)
        DestroyWindow(m_hWnd);

    UnregisterClass(CLASS_NAME, m_hInstance);
}

void Window::ProcessMessages() const {
    MSG msg = {};

    while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::SetCloseCallback(CloseCallback callback) {
    m_closeCallback = callback;
}

LRESULT CALLBACK Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        if (m_closeCallback)
            m_closeCallback();

        DestroyWindow(m_hWnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<Window*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->m_hWnd = hWnd;
    } else {
        window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (window)
        return window->HandleMessage(uMsg, wParam, lParam);

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}