#pragma once

#include "CommandQueue.h"
#include "DescriptorHeap.h"

/* SwapChain */
/* Wraps a swap chain (IDXGISwapChain3) */
/* Manages back buffer rendering and presentation to the window */
class SwapChain
{
public:
    /* BackBuffer */
    /* Represents a single swap chain back buffer */
    struct BackBuffer
    {
        ComPtr<ID3D12Resource> resource;
        D3D12_CPU_DESCRIPTOR_HANDLE rtv;
    };

    SwapChain(const Device& device, const CommandQueue& queue, DescriptorHeap& rtvHeap, HWND hWnd, UINT width, UINT height, UINT bufferCount, DXGI_FORMAT format);

    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;

    void Present(bool vsync);

    inline const BackBuffer& GetCurrentBackBuffer() { return m_backBuffers[m_swapChain->GetCurrentBackBufferIndex()]; } 

private:
    void CreateRTVs(const Device& device, DescriptorHeap& rtvHeap, UINT bufferCount);

private:
    static constexpr UINT MaxBufferCount = 3;

    ComPtr<IDXGISwapChain3> m_swapChain;
    BackBuffer m_backBuffers[MaxBufferCount];

    DXGI_FORMAT m_format;
};