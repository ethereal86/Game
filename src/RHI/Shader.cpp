#include <pch.h>
#include "Shader.h"

#include "Core/Assert.h"

Shader::Shader(const std::string& filename)
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    std::string exeDir(exePath);
    exeDir = exeDir.substr(0, exeDir.find_last_of("\\/"));
    std::string fullPath = exeDir + "/" + filename;

    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        char details[256];
        strerror_s(details, sizeof(details), errno);
        CHECK(false, ("Failed to open shader file '" + fullPath + "': " + details).c_str());
    }

    std::streamsize size = file.tellg();
    CHECK(size > 0, ("Shader file '" + fullPath + "' is empty").c_str());

    file.seekg(0, std::ios::beg);
    
    m_bytecode.resize(static_cast<size_t>(size));

    if (!file.read(m_bytecode.data(), size).good())
    {
        char details[256];
        strerror_s(details, sizeof(details), errno);
        CHECK(false, ("Failed to read shader file '" + fullPath + "': " + details).c_str());
    }
}

D3D12_SHADER_BYTECODE Shader::GetBytecode() const
{
    ASSERT(IsLoaded(), "Shader was not loaded successfully");

    D3D12_SHADER_BYTECODE bytecode = {};
    bytecode.pShaderBytecode = m_bytecode.data();
    bytecode.BytecodeLength = m_bytecode.size();
    return bytecode;
}