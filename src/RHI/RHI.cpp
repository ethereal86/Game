#include <pch.h>
#include "RHI.h"

RHI::RHI(HWND hWnd, UINT width, UINT height)
    : m_graphicsQueue(m_device)
    , m_rtvHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RtvHeapCapacity)
    , m_swapChain(m_device, m_graphicsQueue, m_rtvHeap, hWnd, width, height)
    , m_width(width)
    , m_height(height)
{
    for (UINT i = 0; i < SwapChain::BackBufferCount; ++i)
    {
        DX12_CHECK(
            m_device.Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_frameContexts[i].allocator)), 
            "Failed to create command allocator"
        );
    }

    DX12_CHECK(
        m_device.Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_frameContexts[0].allocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)),
        "Failed to create command list"
    );
    DX12_CHECK(m_commandList->Close(), "Failed to close command list");
}

RHI::~RHI()
{
    m_graphicsQueue.Flush();
}

void RHI::BeginFrame()
{
    FrameContext& frameContext = m_frameContexts[m_frameIndex];

    m_graphicsQueue.WaitForFence(frameContext.fenceValue);

    DX12_CHECK(frameContext.allocator->Reset(), "Failed to reset command allocator");
    DX12_CHECK(m_commandList->Reset(frameContext.allocator.Get(), nullptr), "Failed to reset command list");

    TransitionResource(m_swapChain.GetCurrentBackBuffer().resource.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    SetViewportAndScissor();
}

void RHI::EndFrame(bool vsync)
{
    TransitionResource(m_swapChain.GetCurrentBackBuffer().resource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    DX12_CHECK(m_commandList->Close(), "Failed to close command list");
    m_graphicsQueue.Execute(m_commandList.Get());

    m_swapChain.Present(vsync);

    m_frameContexts[m_frameIndex].fenceValue = m_graphicsQueue.Signal();
    m_frameIndex = (m_frameIndex + 1) % SwapChain::BackBufferCount;
}

void RHI::Flush()
{
    m_graphicsQueue.Flush();
}

RootSignature RHI::CreateRootSignature() const
{
    return RootSignature(m_device);
}

PipelineState RHI::CreatePipelineState(const PipelineDesc& desc) const
{
    return PipelineState(m_device, desc);
}

VertexBuffer RHI::CreateVertexBuffer(const void* data, UINT size, UINT stride) const
{
    return VertexBuffer(m_device, data, size, stride);
}

IndexBuffer RHI::CreateIndexBuffer(const uint16_t* data, UINT size) const
{
    return IndexBuffer(m_device, data, size);
}

void RHI::ClearRenderTarget(const FLOAT color[4]) const
{
    const SwapChain::BackBuffer& backBuffer = m_swapChain.GetCurrentBackBuffer();
    m_commandList->OMSetRenderTargets(1, &backBuffer.rtv, FALSE, nullptr);
    m_commandList->ClearRenderTargetView(backBuffer.rtv, color, 0, nullptr);
}

void RHI::SetRootSignature(const RootSignature& rootSignature) const
{
    m_commandList->SetGraphicsRootSignature(rootSignature.Get());
}

void RHI::SetPipelineState(const PipelineState& pipelineState) const
{
    m_commandList->SetPipelineState(pipelineState.Get());
}

void RHI::SetVertexBuffer(const VertexBuffer& vertexBuffer) const
{
    m_commandList->IASetVertexBuffers(0, 1, &vertexBuffer.GetView());
}

void RHI::SetIndexBuffer(const IndexBuffer& indexBuffer) const
{
    m_commandList->IASetIndexBuffer(&indexBuffer.GetView());
}

void RHI::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const
{
    m_commandList->IASetPrimitiveTopology(topology);
}

void RHI::Draw(UINT vertexCount, UINT instanceCount, UINT startVertex, UINT startInstance) const
{
    m_commandList->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
}

void RHI::DrawIndexed(UINT indexCount, UINT instanceCount, UINT startVertex, INT baseVertex, UINT startInstance) const
{
    m_commandList->DrawIndexedInstanced(indexCount, instanceCount, startVertex, baseVertex, startInstance);
}

void RHI::TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) const
{
    if (stateBefore == stateAfter)
        return;

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type  = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource   = resource;
    barrier.Transition.StateBefore = stateBefore;
    barrier.Transition.StateAfter  = stateAfter;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    m_commandList->ResourceBarrier(1, &barrier);
}

void RHI::SetViewportAndScissor() const
{
    D3D12_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<FLOAT>(m_width);
    viewport.Height = static_cast<FLOAT>(m_height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    D3D12_RECT scissorRect = {};
    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = static_cast<LONG>(m_width);
    scissorRect.bottom = static_cast<LONG>(m_height);

    m_commandList->RSSetViewports(1, &viewport);
    m_commandList->RSSetScissorRects(1, &scissorRect);
}