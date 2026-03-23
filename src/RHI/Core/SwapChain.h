#pragma once

#include "CommandQueue.h"
#include "RHI/Resource/DescriptorHeap.h"

/* Wraps a swap chain (IDXGISwapChain3) */
/* Manages back buffer rendering and presentation to the window */
class SwapChain
{
public:
    /* Represents a single swap chain back buffer */
    struct BackBuffer
    {
        ComPtr<ID3D12Resource> resource;
        D3D12_CPU_DESCRIPTOR_HANDLE rtv;
    };

public:
    static constexpr UINT BackBufferCount = 2;
    static constexpr DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

public:
    SwapChain(const Device& device, const CommandQueue& queue, DescriptorHeap& rtvHeap, HWND hWnd, UINT width, UINT height);

    SwapChain(const SwapChain&) = delete;
    SwapChain& operator=(const SwapChain&) = delete;

    SwapChain(SwapChain&&) = delete;
    SwapChain& operator=(SwapChain&&) = delete;

    void Present(bool vsync);

    inline const BackBuffer& GetCurrentBackBuffer() const { return m_backBuffers[m_swapChain->GetCurrentBackBufferIndex()]; } 

private:
    void CreateRTVs(const Device& device, DescriptorHeap& rtvHeap);

private:
    ComPtr<IDXGISwapChain3> m_swapChain;
    BackBuffer m_backBuffers[BackBufferCount];
};