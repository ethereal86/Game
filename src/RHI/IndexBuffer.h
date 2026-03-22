#pragma once

#include "Device.h"

/* Holds index data on the GPU */
class IndexBuffer
{
public:
    IndexBuffer(const Device& device, const uint16_t* data, UINT size);

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&&) = default;
    IndexBuffer& operator=(IndexBuffer&&) = default;

    inline const D3D12_INDEX_BUFFER_VIEW& GetView() const { return m_view; }
    inline UINT GetIndexCount() const { return m_indexCount; }

private:
    ComPtr<ID3D12Resource> m_resource;
    D3D12_INDEX_BUFFER_VIEW m_view;

    UINT m_indexCount = 0;
};