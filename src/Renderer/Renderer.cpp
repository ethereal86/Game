#include <pch.h>
#include "Renderer.h"

static const float Vertices[] =
{
    -0.5f,  0.5f, 0.0f,         1.0f, 0.8f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,         0.0f, 0.8f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,         0.0f, 0.8f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,         0.0f, 0.8f, 1.0f, 1.0f,
};

static const uint16_t Indices[] = 
{
    0, 1, 2,
    1, 3, 2
};

static const D3D12_INPUT_ELEMENT_DESC InputLayout[] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};

Renderer::Renderer(RHI& rhi)
    : m_rhi(rhi)
    , m_rootSignature(rhi.CreateRootSignature())
{
    Shader vertexShader("shaders/Triangle_VS.cso");
    Shader pixelShader("shaders/Triangle_PS.cso");

    PipelineDesc pipelineDesc;
    pipelineDesc.rootSignature = &m_rootSignature;
    pipelineDesc.vertexShader = &vertexShader;
    pipelineDesc.pixelShader = &pixelShader;
    pipelineDesc.inputLayout = {InputLayout, std::size(InputLayout)};

    m_pipelineState.emplace(rhi.CreatePipelineState(pipelineDesc));
    m_vertexBuffer.emplace(rhi.CreateVertexBuffer(Vertices, sizeof(Vertices), 7 * sizeof(float)));
    m_indexBuffer.emplace(rhi.CreateIndexBuffer(Indices, sizeof(Indices)));
}

void Renderer::Render() const
{
    m_rhi.BeginFrame();

    m_rhi.SetRootSignature(m_rootSignature);
    m_rhi.SetPipelineState(*m_pipelineState);

    constexpr float color[4] = {0.0f, 0.4f, 0.8f, 0.0f};
    m_rhi.ClearRenderTarget(color);

    m_rhi.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_rhi.SetVertexBuffer(*m_vertexBuffer);
    m_rhi.SetIndexBuffer(*m_indexBuffer);

    m_rhi.DrawIndexed(m_indexBuffer->GetIndexCount());

    m_rhi.EndFrame();
}