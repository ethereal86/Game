#include <pch.h>
#include "Renderer.h"

Renderer::Renderer(RHI& rhi)
    : m_rhi(rhi)
{
    Shader vertexShader("shaders/Triangle_VS.cso");
    Shader pixelShader("shaders/Triangle_PS.cso");

    m_pipelineState = &rhi.CreatePipelineState(vertexShader, pixelShader);
}

void Renderer::BeginFrame() const
{
    m_rhi.BeginFrame();
}

void Renderer::EndFrame() const
{
    m_rhi.EndFrame();
}

void Renderer::Render() const
{
    constexpr float color[4] = {0.0f, 0.1f, 0.5f, 0.0f};
    m_rhi.ClearRenderTarget(color);

    ID3D12GraphicsCommandList* list = m_rhi.GetCommandList();
    list->SetPipelineState(m_pipelineState->Get());
    list->SetGraphicsRootSignature(m_rhi.GetRootSignature().Get());

    list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    list->DrawInstanced(3, 1, 0, 0);
}