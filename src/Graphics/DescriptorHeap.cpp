#include <pch.h>
#include "DescriptorHeap.h"

DescriptorAllocator::DescriptorAllocator(DescriptorHeap& heap, UINT startIndex, UINT count) 
    : m_heap(heap), m_startIndex(startIndex), m_count(count ? count : heap.m_numDescriptors - startIndex), m_offset(0)
{ }

DescriptorHandle DescriptorAllocator::Allocate() {
    if (m_offset >= m_count)
        throw std::out_of_range("Descriptor allocator out of range.");

    UINT index = m_startIndex + m_offset++;

    DescriptorHandle handle;
    handle.Cpu = m_heap.m_cpuStart;
    handle.Cpu.ptr += index * m_heap.m_descriptorSize;

    if (m_heap.m_shaderVisible) {
        handle.Gpu = m_heap.m_gpuStart;
        handle.Gpu.ptr += index * m_heap.m_descriptorSize;
    }
    handle.Index = index;
    return handle;
}

DescriptorHeap::DescriptorHeap(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible)
    : m_numDescriptors(numDescriptors), m_shaderVisible(shaderVisible)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = numDescriptors;
    desc.Type = type;   
    desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    if (FAILED(device.Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap))))
        throw std::runtime_error("Failed to create D3D12 descriptor heap.");

    m_descriptorSize = device.Get()->GetDescriptorHandleIncrementSize(type);
    m_cpuStart = m_heap->GetCPUDescriptorHandleForHeapStart();
    if (shaderVisible)
        m_gpuStart = m_heap->GetGPUDescriptorHandleForHeapStart();
}