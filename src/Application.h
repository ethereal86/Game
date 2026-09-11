#pragma once

#include "Window.h"

class Application {
public:
    Application(HINSTANCE hInstance);

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void Run() const;

private:
    Window m_window;

    bool m_running = true;
};