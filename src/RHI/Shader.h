#pragma once

/* Loads precompiled shader bytecode (.cso files) and returns D3D12_SHADER_BYTECODE struct for the pipeline state object */
class Shader
{
public:
    Shader(const std::string& filename);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) = delete;
    Shader& operator=(Shader&&) = delete;

    D3D12_SHADER_BYTECODE GetBytecode() const;

private:
    std::vector<char> m_bytecode;
};