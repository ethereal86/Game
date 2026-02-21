#pragma once

#include <windows.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ProcessMessages() const;
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    static constexpr char s_className[] = "GameWindow";

    HWND m_hWnd;
    HINSTANCE m_hInstance;
};