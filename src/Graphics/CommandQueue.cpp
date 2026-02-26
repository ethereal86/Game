#include <pch.h>
#include "CommandQueue.h"

CommandQueue::CommandQueue(const Device& device, D3D12_COMMAND_LIST_TYPE type)
    : m_fenceValue(0)
{
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    if (FAILED(device.Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue))))
        throw std::runtime_error("Failed to create D3D12 command queue.");

    if (FAILED(device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
        throw std::runtime_error("Failed to create D3D12 fence.");

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!m_fenceEvent)
        throw std::runtime_error("Failed to create fence event.");
}

CommandQueue::~CommandQueue() {
    if (m_fenceEvent)
        CloseHandle(m_fenceEvent);
}

void CommandQueue::Execute(const CommandContext& context) const {
    ID3D12CommandList* lists[] = {context.Get()};
    m_queue->ExecuteCommandLists(1, lists);
}
    
UINT64 CommandQueue::Signal() {
    ++m_fenceValue;

    if (FAILED(m_queue->Signal(m_fence.Get(), m_fenceValue)))
        throw std::runtime_error("Failed to signal fence.");

    return m_fenceValue;
}

void CommandQueue::WaitForFence(UINT64 fenceValue) const {
    if (m_fence->GetCompletedValue() >= fenceValue)
        return;

    if (FAILED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent)))
        throw std::runtime_error("Failed to set fence event.");

    WaitForSingleObject(m_fenceEvent, INFINITE);
}

void CommandQueue::Flush() {
    WaitForFence(Signal());
}