#pragma once

#include "Device.h"

/* CommandQueue */
/* Wraps a command queue (ID3D12CommandQueue) and fence (ID3D12Fence) */
/* Submits recorded command lists to the GPU for execution and owns a fence for CPU/GPU synchronization */
class CommandQueue
{
public:
    CommandQueue(const Device& device);
    ~CommandQueue();

    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator=(const CommandQueue&) = delete;
    
    void Execute(ID3D12GraphicsCommandList* list) const;
    UINT64 Signal();
    void WaitForFence(UINT64 fenceValue) const;
    void Flush();

    inline ID3D12CommandQueue* Get() const { return m_queue.Get(); }
    
private:
    ComPtr<ID3D12CommandQueue> m_queue;
    ComPtr<ID3D12Fence> m_fence;

    HANDLE m_fenceEvent = nullptr;
    UINT64 m_fenceValue = 0;
};