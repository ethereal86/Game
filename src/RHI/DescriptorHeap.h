#pragma once

#include "Device.h"

/* Wraps a descriptor heap (ID3D12DescriptorHeap) */
/* Manages CPU/GPU descriptor handles with a simple linear allocator */
class DescriptorHeap
{
public:
    DescriptorHeap(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t capacity);

    DescriptorHeap(const DescriptorHeap&) = delete;
    DescriptorHeap& operator=(const DescriptorHeap&) = delete;

    DescriptorHeap(DescriptorHeap&&) = delete;
    DescriptorHeap& operator=(DescriptorHeap&&) = delete;

    D3D12_CPU_DESCRIPTOR_HANDLE Allocate();
    
    void Reset();

private:
    ComPtr<ID3D12DescriptorHeap> m_heap;

    D3D12_CPU_DESCRIPTOR_HANDLE m_cpuStart;

    uint32_t m_descriptorSize = 0;
    uint32_t m_capacity = 0;
    uint32_t m_currentIndex = 0;
};