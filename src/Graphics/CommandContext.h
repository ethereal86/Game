#pragma once

#include "Device.h"

class CommandContext {
public:
    CommandContext(const Device& device, D3D12_COMMAND_LIST_TYPE type);

    CommandContext(const CommandContext&) = delete;
    CommandContext& operator=(const CommandContext&) = delete;

    inline ID3D12GraphicsCommandList* Get() const { return m_list.Get(); }

    void Reset() const;
    void Close() const;

    void TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) const;
    
private:
    ComPtr<ID3D12CommandAllocator> m_allocator;
    ComPtr<ID3D12GraphicsCommandList> m_list;
};

struct FrameContext {
    FrameContext(const Device& device)
        : Context(device, D3D12_COMMAND_LIST_TYPE_DIRECT)
    { }

    CommandContext Context;
    UINT64 FenceValue = 0;
};