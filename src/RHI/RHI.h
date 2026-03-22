#pragma once

#include "SwapChain.h"
#include "PipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* Abstraction layer that sits between the renderer and DirectX */
/* Owns all frame-level GPU resources eand manages low-level GPU interaction */
class RHI 
{
public:
    RHI(HWND hWnd, UINT width, UINT height);
    ~RHI();

    RHI(const RHI&) = delete;
    RHI& operator=(const RHI&) = delete;

    RHI(RHI&&) = delete;
    RHI& operator=(RHI&&) = delete;

    void BeginFrame();
    void EndFrame(bool vsync = true);

    void Flush();
    
    RootSignature CreateRootSignature() const;
    PipelineState CreatePipelineState(const PipelineDesc& desc) const;
    VertexBuffer CreateVertexBuffer(const void* data, UINT size, UINT stride) const;
    IndexBuffer CreateIndexBuffer(const uint16_t* data, UINT size) const;
    
    void ClearRenderTarget(const FLOAT color[4]) const;
    void SetRootSignature(const RootSignature& rootSignature) const;
    void SetPipelineState(const PipelineState& pipelineState) const;
    void SetVertexBuffer(const VertexBuffer& vertexBuffer) const;
    void SetIndexBuffer(const IndexBuffer& indexBuffer) const;
    void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const;
    void Draw(UINT vertexCount, UINT instanceCount = 1, UINT startVertex = 0, UINT startInstance = 0) const;
    void DrawIndexed(UINT indexCount, UINT instanceCount = 1, UINT startVertex = 0, INT baseVertex = 0, UINT startInstance = 0) const;

private:
    void TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) const;
    void SetViewportAndScissor() const;

private:
    /* Represents one frame in flight on the GPU */
    struct FrameContext
    {
        ComPtr<ID3D12CommandAllocator> allocator;
        UINT64 fenceValue = 0;
    };

    static constexpr UINT RtvHeapCapacity = 16;

    Device m_device;
    CommandQueue m_graphicsQueue;

    DescriptorHeap m_rtvHeap;
    SwapChain m_swapChain;

    FrameContext m_frameContexts[SwapChain::BackBufferCount];
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    UINT m_frameIndex = 0;

    UINT m_width = 0;
    UINT m_height = 0;
};