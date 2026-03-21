#pragma once

#include "RHI/RHI.h"

/* Renderer */
/* High-level renderer */
/* Owns the reference to the pipeline state created by RHI, drives the frame, and issues draw calls */
class Renderer
{
public:
    Renderer(RHI& rhi);

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void BeginFrame() const;
    void EndFrame() const;

    void Render() const;

private:
    RHI& m_rhi;
    PipelineState* m_pipelineState = nullptr;
};