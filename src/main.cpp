#include "Window.h"
#include "Device.h"

#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Window window(1200, 900, L"Game");
        Device device;
        while (window.ProcessMessages());
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}