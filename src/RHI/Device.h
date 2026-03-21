#pragma once

/* Device */
/* Wraps a DirectX 12 graphics device (ID3D12Device) */
/* Represents the GPU and responsible for creating GPU resources and objects */
class Device 
{
public:
    Device();

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    inline ID3D12Device* Get() const { return m_device.Get(); }
    inline IDXGIFactory4* GetFactory() const { return m_factory.Get(); }

private:
    void SelectAdapter(IDXGIAdapter1** adapter) const;
#ifdef _DEBUG
    void PrintAdapterInfo(IDXGIAdapter1* adapter) const;
#endif

private:
    ComPtr<ID3D12Device> m_device;
    ComPtr<IDXGIFactory4> m_factory;
};