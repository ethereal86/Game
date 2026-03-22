#pragma once

/* Wraps a Win32 window (HWND) */
/* Handles window creation and message processing */
class Window 
{
public:
    Window(int width, int height, const wchar_t* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool ProcessMessages() const;
    void Show() const;

    inline HWND Get() const { return m_hWnd; }
    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }
    
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
private:
    static constexpr wchar_t ClassName[] = L"GameWindow";
    
    HWND m_hWnd = nullptr;

    int m_width = 0;
    int m_height = 0;
};