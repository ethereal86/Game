#include <pch.h>
#include "Application.h"

Application::Application(HINSTANCE hInstance) 
    : m_window(hInstance, 1200, 900, "Game")
{ 
    m_window.SetCloseCallback([this](){
        m_running = false;
    });
}

void Application::Run() const {
    while (m_running) {
        m_window.ProcessMessages();
    }
}