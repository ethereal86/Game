#include <pch.h>
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Device& device, const void* data, UINT size, UINT stride)
{
    ASSERT(data != nullptr, "VertexBuffer data cannot be null");
    ASSERT(size > 0, "VertexBuffer size must be greater than zero");
    ASSERT(stride > 0, "VertexBuffer stride must be greater than zero");

    m_vertexCount = size / stride;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    DX12_CHECK(
        device.Get()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource)),
        "Failed to create vertex buffer"
    );

    void* mapped = nullptr;
    D3D12_RANGE readRange = {0, 0};
    DX12_CHECK(m_resource->Map(0, &readRange, &mapped), "Failed to map vertex buffer");
    memcpy(mapped, data, size);
    m_resource->Unmap(0, nullptr);

    m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
    m_view.SizeInBytes = size;
    m_view.StrideInBytes = stride;
}