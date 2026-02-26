#include <pch.h>
#include "CommandContext.h"

CommandContext::CommandContext(const Device& device, D3D12_COMMAND_LIST_TYPE type) {
    if (FAILED(device.Get()->CreateCommandAllocator(type, IID_PPV_ARGS(&m_allocator))))
        throw std::runtime_error("Failed to create D3D12 command allocator.");

    if (FAILED(device.Get()->CreateCommandList(0, type, m_allocator.Get(), nullptr, IID_PPV_ARGS(&m_list))))
        throw std::runtime_error("Failed to create ID3D12 graphics command list.");

    Close();
}

void CommandContext::Reset() const {
    if (FAILED(m_allocator->Reset()))
        throw std::runtime_error("Failed to reset D3D12 command allocator.");

    if (FAILED(m_list->Reset(m_allocator.Get(), nullptr)))
        throw std::runtime_error("Failed to reset ID3D12 graphics command list.");
}

void CommandContext::Close() const {
    if (FAILED(m_list->Close()))
        throw std::runtime_error("Failed to close ID3D12 graphics command list.");
}

void CommandContext::TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) const {
    if (stateBefore == stateAfter)
        return;

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type  = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource   = resource;
    barrier.Transition.StateBefore = stateBefore;
    barrier.Transition.StateAfter  = stateAfter;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    m_list->ResourceBarrier(1, &barrier);
}