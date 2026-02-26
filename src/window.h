#pragma once

class Window {
public:
    Window(int width, int height, const wchar_t* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    HWND Get() const { return m_hWnd; }

    bool ProcessMessages() const;
    void Show() const;

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    
private:
    static constexpr wchar_t s_className[] = L"GameWindow";

    HWND m_hWnd;
};