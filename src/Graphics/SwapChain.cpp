#include <pch.h>
#include "SwapChain.h"

SwapChain::SwapChain(const Device& device, const CommandQueue& queue, HWND hWnd, int width, int height, UINT bufferCount) {
    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = bufferCount;
    desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    IDXGIFactory6* factory = device.GetFactory();
    ComPtr<IDXGISwapChain1> swapChain;

    if (FAILED(factory->CreateSwapChainForHwnd(queue.Get(), hWnd, &desc, nullptr, nullptr, &swapChain)))
        throw std::runtime_error("Failed to create DXGI swap chain.");

    if (FAILED(swapChain.As(&m_swapChain)))
        throw std::runtime_error("Failed to cast IDXGISwapChain1 to IDXGISwapChain4.");

    m_rtvHeap = std::make_unique<DescriptorHeap>(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount);
    m_rtvAllocator = std::make_unique<DescriptorAllocator>(*m_rtvHeap);

    m_backBuffers.resize(bufferCount);
    for (UINT i = 0; i < bufferCount; ++i)
    {
        ComPtr<ID3D12Resource> buffer;
        if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&buffer))))
            throw std::runtime_error("Failed to get back buffer.");

        m_backBuffers[i].Resource = buffer;
        m_backBuffers[i].RtvHandle = m_rtvAllocator->Allocate();
        device.Get()->CreateRenderTargetView(buffer.Get(), nullptr, m_backBuffers[i].RtvHandle.Cpu);
    }
}

void SwapChain::Present(UINT syncInterval, UINT flags)
{
    m_swapChain->Present(syncInterval, flags);
}