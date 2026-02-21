#include "Device.h"

#include <stdexcept>
#include <iostream>

Device::Device() {
    InitDebugLayer();
    CreateFactory();
    SelectAdapter();

    if (FAILED(D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
        throw std::runtime_error("Failed to create D3D12 Device");
}

void Device::InitDebugLayer() const {
#if defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        debugController->EnableDebugLayer();
#endif
}

void Device::CreateFactory() {
    UINT flags = 0;

#if defined(_DEBUG)
    flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

    if (FAILED(CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory))))
        throw std::runtime_error("Failed to create DXGI Factory");
}

void Device::SelectAdapter() {
    ComPtr<IDXGIAdapter1> bestAdapter;
    SIZE_T maxDedicatedVideoMemory = 0;

    for (UINT i = 0; m_factory->EnumAdapters1(i, &m_adapter) != DXGI_ERROR_NOT_FOUND; i++) {
        DXGI_ADAPTER_DESC1 desc;
        m_adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;

        if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory) {
            maxDedicatedVideoMemory = desc.DedicatedVideoMemory;
            bestAdapter = m_adapter;
        }
    }
    if (!bestAdapter)
        throw std::runtime_error("No compatible hardware found to run DirectX 12");

    m_adapter = bestAdapter;
    PrintAdapterInfo();
}

void Device::PrintAdapterInfo() const {
#if defined(_DEBUG)
    DXGI_ADAPTER_DESC1 desc;
    m_adapter->GetDesc1(&desc);

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