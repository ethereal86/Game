#pragma once

/* Shader */
/* Loads precompiled shader bytecode (.cso files) and returns D3D12_SHADER_BYTECODE struct for the pipeline state object */
class Shader
{
public:
    Shader(const std::string& filename);

    D3D12_SHADER_BYTECODE GetBytecode() const;
    
    inline bool IsLoaded() const { return !m_bytecode.empty(); }

private:
    std::vector<char> m_bytecode;
};