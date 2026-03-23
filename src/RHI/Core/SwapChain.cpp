#include <pch.h>
#include "SwapChain.h"

SwapChain::SwapChain(const Device& device, const CommandQueue& queue, DescriptorHeap& rtvHeap, HWND hWnd, UINT width, UINT height)
{
    ASSERT(width > 0, "SwapChain width must be greater than zero");
    ASSERT(height > 0, "SwapChain height must be greater than zero");

    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.Format = BackBufferFormat;
    desc.Stereo = FALSE;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = BackBufferCount;
    desc.Scaling = DXGI_SCALING_NONE;
    desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    ComPtr<IDXGISwapChain1> swapChain1;

    DX12_CHECK(
        device.GetFactory()->CreateSwapChainForHwnd(queue.Get(), hWnd, &desc, nullptr, nullptr, &swapChain1),
        "Failed to create swap chain"
    );
    DX12_CHECK(
        device.GetFactory()->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER),
        "Failed to disable Alt + Enter"
    );
    DX12_CHECK(
        swapChain1.As(&m_swapChain),
        "Failed to query IDXGISwapChain3"
    );

    CreateRTVs(device, rtvHeap);
}

void SwapChain::Present(bool vsync)
{
    UINT syncInterval = vsync ? 1 : 0;
    UINT flags = vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING;

    DX12_CHECK(
        m_swapChain->Present(syncInterval, flags),
        "Failed to present swap chain"
    );
}

void SwapChain::CreateRTVs(const Device& device, DescriptorHeap& rtvHeap)
{
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = BackBufferFormat;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    for (UINT i = 0; i < BackBufferCount; ++i)
    {
        DX12_CHECK(
            m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i].resource)),
            "Failed to get swap chain back buffer"
        );
        m_backBuffers[i].rtv = rtvHeap.Allocate();
        device.Get()->CreateRenderTargetView(m_backBuffers[i].resource.Get(), &rtvDesc, m_backBuffers[i].rtv);
    }
}