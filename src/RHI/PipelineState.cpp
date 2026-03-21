#include <pch.h>
#include "PipelineState.h"

#include "Core/Assert.h"

PipelineState::PipelineState(const Device& device, const RootSignature& rootSignature, const Shader& vertexShader, const Shader& pixelShader, DXGI_FORMAT rtvFormat)
{
    ASSERT(vertexShader.IsLoaded(), "Missing vertex shader bytecode");
    ASSERT(pixelShader.IsLoaded(), "Missing pixel shader bytecode");

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
    desc.pRootSignature = rootSignature.Get();
    desc.VS = vertexShader.GetBytecode();
    desc.PS = pixelShader.GetBytecode();

    D3D12_BLEND_DESC blend = {};
    blend.AlphaToCoverageEnable = FALSE;
    blend.IndependentBlendEnable = FALSE;

    D3D12_RENDER_TARGET_BLEND_DESC rtBlend = {};
    rtBlend.BlendEnable = FALSE;
    rtBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    blend.RenderTarget[0] = rtBlend;

    desc.BlendState = blend;
    desc.SampleMask = UINT_MAX;

    D3D12_RASTERIZER_DESC rasterizer = {};
    rasterizer.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizer.CullMode = D3D12_CULL_MODE_BACK;
    rasterizer.FrontCounterClockwise = FALSE;
    rasterizer.DepthClipEnable = TRUE;

    desc.RasterizerState = rasterizer;

    D3D12_DEPTH_STENCIL_DESC depthStencil = {};
    depthStencil.DepthEnable = FALSE;
    depthStencil.StencilEnable = FALSE;

    desc.DepthStencilState = depthStencil;
    desc.InputLayout = {nullptr, 0};
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = rtvFormat;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    DX12_CHECK(
        device.Get()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipelineState)),
        "Failed to create pipeline state object"
    );
}