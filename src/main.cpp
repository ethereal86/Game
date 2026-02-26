#include <pch.h>

#include "Window.h"
#include "Graphics/SwapChain.h"

int main(int argc, char* argv[]) {
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    try {
        Window window(1200, 900, L"Game");

        int frameCount = 2;

        Device device;
        CommandQueue graphicsQueue(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
        SwapChain swapChain(device, graphicsQueue, window.Get(), 1200, 900, frameCount);
        
        std::vector<std::unique_ptr<FrameContext>> frames;
        for (int i = 0; i < frameCount; ++i)
            frames.emplace_back(std::make_unique<FrameContext>(device));

        window.Show();

        bool running = true;
        while (running) {
            if (!window.ProcessMessages())
                running = false;

            FrameContext& frame = *frames[swapChain.GetCurrentBackBufferIndex()];
            CommandContext& context = frame.Context;
            BackBuffer& buffer = swapChain.GetCurrentBackBuffer();

            if (frame.FenceValue != 0)
                graphicsQueue.WaitForFence(frame.FenceValue);

            context.Reset();

            context.TransitionResource(buffer.Resource.Get(), buffer.State, D3D12_RESOURCE_STATE_RENDER_TARGET);
            buffer.State = D3D12_RESOURCE_STATE_RENDER_TARGET;

            context.Get()->OMSetRenderTargets(1, &buffer.RtvHandle.Cpu, FALSE, nullptr);
            float clearColor[4] = {0.0f, 0.3f, 0.6f, 0.0f};
            context.Get()->ClearRenderTargetView(buffer.RtvHandle.Cpu, clearColor, 0, nullptr);

            context.TransitionResource(buffer.Resource.Get(), buffer.State, D3D12_RESOURCE_STATE_PRESENT);
            buffer.State = D3D12_RESOURCE_STATE_PRESENT;
            
            context.Close();
            graphicsQueue.Execute(context);
            frame.FenceValue = graphicsQueue.Signal();
            swapChain.Present();
        }
        graphicsQueue.Flush();

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}