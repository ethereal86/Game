#include <cstdio>
#include "window.h"

int main(int argc, char* argv[]) {
    Window window(1200, 900, "Game");
    printf("Win32 window initialized\n");
    while (window.ProcessMessages());
    return 0;
}