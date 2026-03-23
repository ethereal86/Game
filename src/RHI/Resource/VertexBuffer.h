#pragma once

#include "RHI/Core/Device.h"

/* Holds vertex data on the GPU */
class VertexBuffer
{
public:
    VertexBuffer(const Device& device, const void* data, UINT size, UINT stride);

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&&) = default;
    VertexBuffer& operator=(VertexBuffer&&) = default;

    inline const D3D12_VERTEX_BUFFER_VIEW& GetView() const { return m_view; }
    inline UINT GetVertexCount() const { return m_vertexCount; }

private:
    ComPtr<ID3D12Resource> m_resource;
    D3D12_VERTEX_BUFFER_VIEW m_view;
    
    UINT m_vertexCount = 0;
};