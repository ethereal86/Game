#pragma once

#include "Device.h"

class CommandAllocator {
public:
    CommandAllocator(Device& device, D3D12_COMMAND_LIST_TYPE type);
    ~CommandAllocator() = default;

    CommandAllocator(const CommandAllocator&) = delete;
    CommandAllocator& operator=(const CommandAllocator&) = delete;

    inline ID3D12CommandAllocator* Get() const { return m_allocator.Get(); }

    void Reset() const;
private:
    ComPtr<ID3D12CommandAllocator> m_allocator;
};