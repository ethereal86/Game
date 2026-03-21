#include <pch.h>
#include "RootSignature.h"

#include "Core/Assert.h"

RootSignature::RootSignature(const Device& device) 
{
    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters = 0;
    desc.pParameters = nullptr;
    desc.NumStaticSamplers = 0;
    desc.pStaticSamplers = nullptr;
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> serialized;
    ComPtr<ID3DBlob> error;

    HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error);
    if (FAILED(hr)) 
    {
        const char* details = (error && error->GetBufferSize() > 0) 
            ? static_cast<const char*>(error->GetBufferPointer())
            : "No error details available";

        INTERNAL_FATAL("D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error)", "Failed to serialize root signature: %s", details);
    }

    DX12_CHECK(
        device.Get()->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)),
        "Failed to create root signature"
    );
}