#pragma once

/* Window */
/* Wraps a Win32 window (HWND) */
/* Handles window creation and message processing */
class Window 
{
public:
    Window(int width, int height, const wchar_t* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ProcessMessages() const;
    void Show() const;

    inline HWND Get() const { return m_hWnd; }
    
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
private:
    static constexpr wchar_t WindowClassName[] = L"GameWindow";
    
    HWND m_hWnd = nullptr;
};