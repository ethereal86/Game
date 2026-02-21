#include "CommandAllocator.h"

#include <stdexcept>

CommandAllocator::CommandAllocator(Device& device, D3D12_COMMAND_LIST_TYPE type) {
    if (FAILED(device.Get()->CreateCommandAllocator(type, IID_PPV_ARGS(&m_allocator))))
        throw std::runtime_error("Failed to create D3D12 Command Allocator");
}

void CommandAllocator::Reset() const {
    if (FAILED(m_allocator->Reset()))
        throw std::runtime_error("Failed to reset D3D12 Command Allocator");
}