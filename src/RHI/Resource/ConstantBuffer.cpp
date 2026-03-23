#include <pch.h>
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(const Device& device, UINT size)
{
    ASSERT(size > 0, "ConstantBuffer size must be greater than zero");

    m_alignedSize = (size + 255) & ~255;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = m_alignedSize;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    DX12_CHECK(
        device.Get()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource)),
        "Failed to create constant buffer"
    );

    D3D12_RANGE readRange = {0, 0};
    DX12_CHECK(m_resource->Map(0, &readRange, &m_mappedData), "Failed to map constant buffer");
}

ConstantBuffer::~ConstantBuffer()
{
    if (m_resource)
        m_resource->Unmap(0, nullptr);

    m_mappedData = nullptr;
}

void ConstantBuffer::Update(const void* data, UINT size) const
{
    ASSERT(data != nullptr, "ConstantBuffer data cannot be null");
    ASSERT(size <= m_alignedSize, "ConstantBuffer data size exceeds capacity");
 
    memcpy(m_mappedData, data, size);
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGPUAddress() const
{
    return m_resource->GetGPUVirtualAddress();
}