#include <pch.h>
#include "Renderer.h"

using namespace DirectX;

static const float Vertices[] =
{
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f,
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
    pipelineDesc.inputLayout = {InputLayout, static_cast<UINT>(std::size(InputLayout))};

    m_pipelineState.emplace(rhi.CreatePipelineState(pipelineDesc));
    m_vertexBuffer.emplace(rhi.CreateVertexBuffer(Vertices, sizeof(Vertices), 7 * sizeof(float)));
    m_indexBuffer.emplace(rhi.CreateIndexBuffer(Indices, sizeof(Indices)));
    m_constantBuffer.emplace(rhi.CreateConstantBuffer(sizeof(XMFLOAT4X4)));
}

void Renderer::Render()
{
    m_rhi.BeginFrame();

    XMMATRIX model = XMMatrixIdentity();
    model *= XMMatrixTranslation(sin(m_frameCount * 0.05f) * 0.6f, cos(m_frameCount * 0.05f) * 0.6f, 0.0f);
    model *= XMMatrixRotationZ(m_frameCount * 0.02f);
    model *= XMMatrixScaling(0.6f, 0.6f, 0.6f);

    CBuffer cbuffer;
    XMStoreFloat4x4(&cbuffer.modelMatrix, XMMatrixTranspose(model));
    m_constantBuffer->Update(&cbuffer, sizeof(cbuffer));

    m_rhi.SetRootSignature(m_rootSignature);
    m_rhi.SetPipelineState(*m_pipelineState);
    m_rhi.SetConstantBuffer(*m_constantBuffer, 0);

    constexpr float color[4] = {0.2f, 0.5f, 1.0f, 0.0f};
    m_rhi.ClearRenderTarget(color);

    m_rhi.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_rhi.SetVertexBuffer(*m_vertexBuffer);
    m_rhi.SetIndexBuffer(*m_indexBuffer);

    m_rhi.DrawIndexed(m_indexBuffer->GetIndexCount());

    m_rhi.EndFrame();

    m_frameCount++;
}