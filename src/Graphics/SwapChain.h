#pragma once

#include "CommandQueue.h"
#include "CommandContext.h"
#include "DescriptorHeap.h"

struct BackBuffer {
    ComPtr<ID3D12Resource> Resource;
    DescriptorHandle RtvHandle;
    D3D12_RESOURCE_STATES State = D3D12_RESOURCE_STATE_PRESENT;
};

class SwapChain {
public:
    SwapChain(const Device& device, const CommandQueue& queue, HWND hWnd, int width, int height, UINT bufferCount = 2);

    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;

    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHandle() const;

    inline UINT GetCurrentBackBufferIndex() const { return m_swapChain->GetCurrentBackBufferIndex(); }
    inline BackBuffer& GetCurrentBackBuffer() { return m_backBuffers[GetCurrentBackBufferIndex()]; } 

    void Present(UINT syncInterval = 1, UINT flags = 0);

private:
    ComPtr<IDXGISwapChain4> m_swapChain;
    std::unique_ptr<DescriptorHeap> m_rtvHeap;
    std::unique_ptr<DescriptorAllocator> m_rtvAllocator;

    std::vector<BackBuffer> m_backBuffers;
};