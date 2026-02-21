#include "Window.h"
#include "Device.h"

#include "CommandQueue.h"
#include "CommandList.h"

#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Window window(1200, 900, L"Game");
        Device device;

        CommandQueue graphicsQueue(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
        CommandAllocator allocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
        CommandList list(device, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator);

        allocator.Reset();
        list.Reset(allocator);
        list.Close();

        ID3D12CommandList* lists[] = {list.Get()};
        graphicsQueue.Execute(lists, 1);
        graphicsQueue.Flush();
        allocator.Reset();

        while (window.ProcessMessages());

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}