#pragma once

#include "CommandAllocator.h"

class CommandList {
public:
    CommandList(Device& device, D3D12_COMMAND_LIST_TYPE type, CommandAllocator& allocator);
    ~CommandList() = default;

    CommandList(const CommandList&) = delete;
    CommandList& operator=(const CommandList&) = delete;

    inline ID3D12GraphicsCommandList* Get() const { return m_list.Get(); }

    void Reset(CommandAllocator& allocator) const;
    void Close() const;
private:
    ComPtr<ID3D12GraphicsCommandList> m_list;
};