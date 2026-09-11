#pragma once

class Window {
public:
    using CloseCallback = std::function<void()>;

public:
    Window(HINSTANCE hInstance, int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    void ProcessMessages() const;
    
    void SetCloseCallback(CloseCallback callback);

private:
    LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    CloseCallback m_closeCallback;

    static constexpr const char* CLASS_NAME = "GameWindow";
};