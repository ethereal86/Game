#pragma once

#include "RHI/RHI.h"

/* High-level renderer */
/* Owns the reference to the pipeline state created by RHI, drives the frame, and issues draw calls */
class Renderer
{
public:
    Renderer(RHI& rhi);

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void Render();

private:
    /* Block of data that gets uploaded to the constant buffer */
    struct CBuffer
    {
        DirectX::XMFLOAT4X4 modelMatrix;
    };

private:
    RHI& m_rhi;

    RootSignature m_rootSignature;
    std::optional<PipelineState> m_pipelineState;

    std::optional<VertexBuffer> m_vertexBuffer;
    std::optional<IndexBuffer> m_indexBuffer;
    std::optional<ConstantBuffer> m_constantBuffer;

    UINT m_frameCount = 0;
};