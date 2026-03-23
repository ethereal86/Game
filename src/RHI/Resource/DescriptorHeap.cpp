#include <pch.h>
#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t capacity)
    : m_capacity(capacity)
{
    ASSERT(capacity > 0, "Descriptor heap capacity must be greater than zero");

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = capacity;
    desc.Type = type;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = 0;

    DX12_CHECK(
        device.Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap)),
        "Failed to create descriptor heap"
    );

    m_cpuStart = m_heap->GetCPUDescriptorHandleForHeapStart();
    m_descriptorSize = device.Get()->GetDescriptorHandleIncrementSize(type);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::Allocate()
{
    CHECK(m_currentIndex < m_capacity, "Descriptor heap is full");
 
    D3D12_CPU_DESCRIPTOR_HANDLE handle;
    handle.ptr = m_cpuStart.ptr + m_currentIndex * m_descriptorSize;
 
    ++m_currentIndex;
    return handle;
}

void DescriptorHeap::Reset()
{
    m_currentIndex = 0;
}