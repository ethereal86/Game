#pragma once

#include "Device.h"

struct DescriptorHandle {
    D3D12_CPU_DESCRIPTOR_HANDLE Cpu;
    D3D12_GPU_DESCRIPTOR_HANDLE Gpu;
    UINT Index = 0;
};

class DescriptorHeap;

class DescriptorAllocator {
public:
    DescriptorAllocator(DescriptorHeap& heap, UINT startIndex = 0, UINT count = 0);

    DescriptorAllocator(const DescriptorAllocator&) = delete;
    DescriptorAllocator& operator=(const DescriptorAllocator&) = delete;

    inline void Reset() { m_offset = 0; }

    DescriptorHandle Allocate();

private:
    DescriptorHeap& m_heap;
    UINT m_startIndex;
    UINT m_count;
    UINT m_offset;
};

class DescriptorHeap {
public:
    DescriptorHeap(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false);

    DescriptorHeap(const DescriptorHeap&) = delete;
    DescriptorHeap& operator=(const DescriptorHeap&) = delete;

    inline ID3D12DescriptorHeap* Get() const { return m_heap.Get(); }

private:
    ComPtr<ID3D12DescriptorHeap> m_heap;

    D3D12_CPU_DESCRIPTOR_HANDLE m_cpuStart;
    D3D12_GPU_DESCRIPTOR_HANDLE m_gpuStart;

    bool m_shaderVisible;
    UINT m_descriptorSize;
    UINT m_numDescriptors;

    friend class DescriptorAllocator;
};