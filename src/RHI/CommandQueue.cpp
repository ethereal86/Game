#include <pch.h>
#include "CommandQueue.h"

#include "Core/Assert.h"

CommandQueue::CommandQueue(const Device& device)
{
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    DX12_CHECK(
        device.Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue)), 
        "Failed to create command queue"
    );
    DX12_CHECK(
        device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)),
        "Failed to create fence"
    );

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    WIN32_CHECK(m_fenceEvent != nullptr, "Failed to create fence event");
}

CommandQueue::~CommandQueue()
{
    Flush();

    if (m_fenceEvent)
        CloseHandle(m_fenceEvent);
}

void CommandQueue::Execute(ID3D12GraphicsCommandList* list) const
{
    ASSERT(list != nullptr, "Command list is null");

    ID3D12CommandList* lists[] = {list};
    m_queue->ExecuteCommandLists(1, lists);
}

UINT64 CommandQueue::Signal()
{
    DX12_CHECK(
        m_queue->Signal(m_fence.Get(), ++m_fenceValue),
        "Failed to signal command queue"
    );
    return m_fenceValue;
}

void CommandQueue::WaitForFence(UINT64 fenceValue) const
{
    if (m_fence->GetCompletedValue() >= fenceValue)
        return;

    DX12_CHECK(
        m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent),
        "Failed to set fence event"
    );
    WaitForSingleObject(m_fenceEvent, INFINITE);
}

void CommandQueue::Flush()
{
    WaitForFence(Signal());
}