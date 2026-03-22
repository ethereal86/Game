#include <pch.h>
#include "Device.h"

Device::Device() 
{
    UINT flags = 0;

#ifdef _DEBUG
    ComPtr<ID3D12Debug> debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) 
    {
        debug->EnableDebugLayer();
        flags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    DX12_CHECK(
        CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory)), 
        "Failed to create DXGI factory"
    );

    ComPtr<IDXGIAdapter1> adapter;
    SelectAdapter(&adapter);
    
    DX12_CHECK(
        D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)),
        "Failed to create DX12 device"
    );

#ifdef _DEBUG
    ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(m_device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) 
    {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
    }

    PrintAdapterInfo(adapter.Get());
#endif
}

void Device::SelectAdapter(IDXGIAdapter1** adapter) const
{
    ComPtr<IDXGIAdapter1> bestAdapter;
    SIZE_T maxDedicatedVideoMemory = 0;

    ComPtr<IDXGIAdapter1> currentAdapter;
    for (UINT i = 0; m_factory->EnumAdapters1(i, &currentAdapter) != DXGI_ERROR_NOT_FOUND; ++i) 
    {
        DXGI_ADAPTER_DESC1 desc;
        currentAdapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;

        if (FAILED(D3D12CreateDevice(currentAdapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
            continue;

        if (desc.DedicatedVideoMemory > maxDedicatedVideoMemory)
        {
            maxDedicatedVideoMemory = desc.DedicatedVideoMemory;
            bestAdapter = currentAdapter;
        }
    }

    CHECK(bestAdapter != nullptr, "No DirectX 12 compatible GPU found");
    *adapter = bestAdapter.Detach();
}

#ifdef _DEBUG
void Device::PrintAdapterInfo(IDXGIAdapter1* adapter) const
{
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);

    const char* vendor = "Unknown";
    switch (desc.VendorId)
    {
        case 0x10DE: vendor = "NVIDIA"; break;
        case 0x1002: vendor = "AMD";    break;
        case 0x8086: vendor = "Intel";  break;
    }

    char name[128];
    WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, name, sizeof(name), nullptr, nullptr);

    char info[512];
    snprintf(info, sizeof(info),
        "=================================================\n"
        "                Selected Hardware                \n"
        "=================================================\n"
        "   Name           : %s\n"
        "   Vendor         : %s (ID = 0x%04x)\n"
        "   Device ID      : 0x%04x\n"
        "   Dedicated VRAM : %zu MB\n"
        "   Dedicated RAM  : %zu MB\n"
        "   Shared RAM     : %zu MB\n"
        "\n",
        name,
        vendor, desc.VendorId,
        desc.DeviceId,
        desc.DedicatedVideoMemory / (1024 * 1024),
        desc.DedicatedSystemMemory / (1024 * 1024),
        desc.SharedSystemMemory / (1024 * 1024)
    );
    OutputDebugStringA(info);
}
#endif