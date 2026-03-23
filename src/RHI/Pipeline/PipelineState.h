#pragma once

#include "RootSignature.h"
#include "Shader.h"

/* Minimum required fields to create a pipeline state object */
struct PipelineDesc
{
    const RootSignature* rootSignature = nullptr;
    const Shader* vertexShader = nullptr;
    const Shader* pixelShader = nullptr;
    D3D12_INPUT_LAYOUT_DESC inputLayout;
};

/* Wraps a pipeline state object (ID3D12PipelineState) */
/* Represents the entire configuration of a graphics pipeline (shaders, blend modes, rasterizer state, depth/stencil settings) */
class PipelineState {
public:
    PipelineState(const Device& device, const PipelineDesc& pipelineDesc);

    PipelineState(const PipelineState&) = delete;
    PipelineState& operator=(const PipelineState&) = delete;

    PipelineState(PipelineState&&) = default;
    PipelineState& operator=(PipelineState&&) = default;

    inline ID3D12PipelineState* Get() const { return m_pipelineState.Get(); }

private:
    ComPtr<ID3D12PipelineState> m_pipelineState;
};