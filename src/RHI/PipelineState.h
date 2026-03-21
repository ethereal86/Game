#pragma once

#include "RootSignature.h"
#include "Shader.h"

/* PipelineState */
/* Wraps a pipeline state object (ID3D12PipelineState) */
/* Represents the entire configuration of a graphics pipeline (shaders, blend modes, rasterizer state, depth/stencil settings) */
class PipelineState {
public:
    PipelineState(const Device& device, const RootSignature& rootSignature, const Shader& vertexShader, const Shader& pixelShader, DXGI_FORMAT rtvFormat);

    PipelineState(const PipelineState&) = delete;
    PipelineState& operator=(const PipelineState&) = delete;

    PipelineState(PipelineState&&) = default;
    PipelineState& operator=(PipelineState&&) = default;

    inline ID3D12PipelineState* Get() const { return m_pipelineState.Get(); }

private:
    ComPtr<ID3D12PipelineState> m_pipelineState;
};