#pragma once

#include "RHI/Core/Device.h"

/* Holds read-only data for shaders on the GPU */
class ConstantBuffer
{
public:
    ConstantBuffer(const Device& device, UINT size);
    ~ConstantBuffer();

    ConstantBuffer(const ConstantBuffer&) = delete;
    ConstantBuffer& operator=(const ConstantBuffer&) = delete;

    ConstantBuffer(ConstantBuffer&&) = default;
    ConstantBuffer& operator=(ConstantBuffer&&) = default;

    void Update(const void* data, UINT size) const;

    D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() const;

private:
    ComPtr<ID3D12Resource> m_resource;
    void* m_mappedData = nullptr;

    UINT m_alignedSize = 0;
};