#pragma once

#include "SwapChain.h"
#include "PipelineState.h"

/* RHI */
/* Owns all RHI objects and controls all low-level GPU interaction */
class RHI 
{
public:
    RHI(HWND hWnd, UINT width, UINT height);
    ~RHI();

    RHI(const RHI&) = delete;
    RHI& operator=(const RHI&) = delete;

    void BeginFrame();
    void EndFrame(bool vsync = true);

    void Flush();

    void ClearRenderTarget(const FLOAT color[4]);

    PipelineState& CreatePipelineState(const Shader& vertexShader, const Shader& pixelShader);

    inline const RootSignature& GetRootSignature() { return m_rootSignature; }
    inline ID3D12GraphicsCommandList* GetCommandList() { return m_commandList.Get(); }
    
private:
    void TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) const;
    void SetViewportAndScissor() const;

private:
    /* FrameContext */
    /* Represents one frame in flight on the GPU */
    struct FrameContext
    {
        ComPtr<ID3D12CommandAllocator> allocator;
        UINT64 fenceValue = 0;
    };

    static constexpr UINT FrameCount = 2;
    static constexpr UINT RtvHeapCapacity = 16;
    static constexpr DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

    Device m_device;
    CommandQueue m_graphicsQueue;

    DescriptorHeap m_rtvHeap;
    SwapChain m_swapChain;

    RootSignature m_rootSignature;
    std::vector<PipelineState> m_pipelineStates;

    FrameContext m_frameContexts[FrameCount];
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    UINT m_frameIndex = 0;

    UINT m_width = 0;
    UINT m_height = 0;

    bool m_vsync = false;
};