#pragma once

#include "Device.h"

/* RootSignature */
/* Wraps a root signature (ID3D12RootSignature) */
/* Defines what resources a shader can access */
class RootSignature
{
public:
    RootSignature(const Device& device);

    RootSignature(const RootSignature&) = delete;
    RootSignature& operator=(const RootSignature&) = delete;

    inline ID3D12RootSignature* Get() const { return m_rootSignature.Get(); }

private:
    ComPtr<ID3D12RootSignature> m_rootSignature;
};