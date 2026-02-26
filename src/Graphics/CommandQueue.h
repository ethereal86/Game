#pragma once

#include "Device.h"
#include "CommandContext.h"

class CommandQueue {
public:
    CommandQueue(const Device& device, D3D12_COMMAND_LIST_TYPE type);    
    ~CommandQueue();

    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator=(const CommandQueue&) = delete;

    inline ID3D12CommandQueue* Get() const { return m_queue.Get(); }
    
    void Execute(const CommandContext& context) const;
    UINT64 Signal();
    void WaitForFence(UINT64 fenceValue) const;
    void Flush();
    
private:
    ComPtr<ID3D12CommandQueue> m_queue;

    ComPtr<ID3D12Fence> m_fence;
    HANDLE m_fenceEvent;
    UINT64 m_fenceValue;
};