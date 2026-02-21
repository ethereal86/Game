#include "CommandQueue.h"

#include <stdexcept>

CommandQueue::CommandQueue(Device& device, D3D12_COMMAND_LIST_TYPE type) {
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    if (FAILED(device.Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue))))
        throw std::runtime_error("Failed to create D3D12 Command Queue");

    CreateFence(device);
}

CommandQueue::~CommandQueue() {
    if (m_fenceEvent)
        CloseHandle(m_fenceEvent);
}

void CommandQueue::Execute(ID3D12CommandList* const* lists, UINT count) const {
    m_queue->ExecuteCommandLists(count, lists);
}
    
uint64_t CommandQueue::Signal() {
    uint64_t value = ++m_fenceValue;

    if (FAILED(m_queue->Signal(m_fence.Get(), value)))
        throw std::runtime_error("Failed to signal fence");

    return value;
}

void CommandQueue::WaitForFence(uint64_t fenceValue) const {
    if (m_fence->GetCompletedValue() >= fenceValue)
        return;

    if (FAILED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent)))
        throw std::runtime_error("Failed to set fence event");

    WaitForSingleObject(m_fenceEvent, INFINITE);
}

void CommandQueue::Flush() {
    uint64_t value = Signal();
    WaitForFence(value);
}

void CommandQueue::CreateFence(Device& device) {
    if (FAILED(device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
        throw std::runtime_error("Failed to create D3D12 Fence");

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!m_fenceEvent)
        throw std::runtime_error("Failed to create fence event");
}