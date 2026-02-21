#pragma once

#include "Device.h"

#include <cstdint>

class CommandQueue {
public:
    CommandQueue(Device& device, D3D12_COMMAND_LIST_TYPE type);
    ~CommandQueue();

    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator=(const CommandQueue&) = delete;

    inline ID3D12CommandQueue* Get() const { return m_queue.Get(); }

    void Execute(ID3D12CommandList* const* lists, UINT count) const;
    uint64_t Signal();
    void WaitForFence(uint64_t fenceValue) const;
    void Flush();

private:
    void CreateFence(Device& device);
    
private:
    ComPtr<ID3D12CommandQueue> m_queue;

    ComPtr<ID3D12Fence> m_fence;
    HANDLE m_fenceEvent;
    uint64_t m_fenceValue = 0;
};