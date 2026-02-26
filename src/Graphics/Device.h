#pragma once

using Microsoft::WRL::ComPtr;

class Device {
public:
    Device();

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    inline ID3D12Device* Get() const { return m_device.Get(); }
    inline IDXGIFactory6* GetFactory() const { return m_factory.Get(); }

private:
    ComPtr<IDXGIAdapter1> SelectAdapter(const ComPtr<IDXGIFactory6>& factory) const;
    void PrintAdapterInfo(const ComPtr<IDXGIAdapter1>& adapter) const;

private:
    ComPtr<ID3D12Device> m_device;
    ComPtr<IDXGIFactory6> m_factory;
};