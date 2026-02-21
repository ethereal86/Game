#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d12sdklayers.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class Device {
public:
    Device();
    ~Device() = default;

    ID3D12Device* Get() const { return m_device.Get(); }
    IDXGIFactory6* GetFactory() const { return m_factory.Get(); }
    IDXGIAdapter1* GetAdapter() const { return m_adapter.Get(); }
private:
    void InitDebugLayer() const;
    void CreateFactory();
    void SelectAdapter();
    void PrintAdapterInfo() const;
private:
    ComPtr<ID3D12Device> m_device;
    ComPtr<IDXGIFactory6> m_factory;
    ComPtr<IDXGIAdapter1> m_adapter;
};