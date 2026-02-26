#include <pch.h>
#include "Device.h"

Device::Device() {
    UINT flags = 0;

#if defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        debugController->EnableDebugLayer();

    flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

    if (FAILED(CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory))))
        throw std::runtime_error("Failed to create DXGI factory.");

    ComPtr<IDXGIAdapter1> adapter = SelectAdapter(m_factory);
    if (!adapter)
        throw std::runtime_error("No compatible hardware adapter found.");
    
    if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
        throw std::runtime_error("Failed to create D3D12 device.");

    PrintAdapterInfo(adapter);
}

ComPtr<IDXGIAdapter1> Device::SelectAdapter(const ComPtr<IDXGIFactory6>& factory) const {
    ComPtr<IDXGIAdapter1> bestAdapter;
    SIZE_T maxDedicatedVideoMemory = 0;

    for (UINT i = 0;; ++i) {
        ComPtr<IDXGIAdapter1> adapter;
        if(factory->EnumAdapters1(i, &adapter) == DXGI_ERROR_NOT_FOUND)
            break;

        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;

        if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
            continue;

        if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory) {
            maxDedicatedVideoMemory = desc.DedicatedVideoMemory;
            bestAdapter = adapter;
        }
    }
    return bestAdapter;
}

void Device::PrintAdapterInfo(const ComPtr<IDXGIAdapter1>& adapter) const {
#if defined(_DEBUG)
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);

    std::wcout << "=================================================" << std::endl;
    std::wcout << "                Selected Hardware                " << std::endl;
    std::wcout << "=================================================" << std::endl;
    std::wcout << "Name                 : " << desc.Description << std::endl;
    std::wcout << "Vendor ID            : " << desc.VendorId << std::endl;
    std::wcout << "Device ID            : " << desc.DeviceId << std::endl;
    std::wcout << "Dedicated VRAM       : " << desc.DedicatedVideoMemory / (1024 * 1024) << L" MB" << std::endl;
    std::wcout << "Shared System Memory : " << desc.SharedSystemMemory / (1024 * 1024) << L" MB" << std::endl;
    std::wcout << "Adapter Flags        : " << desc.Flags << std::endl;
#endif
}