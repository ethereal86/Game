#include "CommandList.h"

#include <stdexcept>

CommandList::CommandList(Device& device, D3D12_COMMAND_LIST_TYPE type, CommandAllocator& allocator) {
    if (FAILED(device.Get()->CreateCommandList(0, type, allocator.Get(), nullptr, IID_PPV_ARGS(&m_list))))
        throw std::runtime_error("Failed to create ID3D12 Graphics Command List");

    Close();
}

void CommandList::Reset(CommandAllocator& allocator) const {
    if (FAILED(m_list->Reset(allocator.Get(), nullptr)))
        throw std::runtime_error("Failed to reset ID3D12 Graphics Command List");
}

void CommandList::Close() const {
    if (FAILED(m_list->Close()))
        throw std::runtime_error("Failed to close ID3D12 Graphics Command List");
}