#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

class Shader
{
public:
    Shader() = default;
    ~Shader();

    bool Load(
        ID3D11Device* device,
        const std::wstring& vertexShaderPath,
        const std::wstring& pixelShaderPath);

    void Bind(ID3D11DeviceContext* context) const;

private:
    ID3D11VertexShader* m_vs = nullptr;
    ID3D11PixelShader* m_ps = nullptr;
    ID3D11InputLayout* m_inputLayout = nullptr;
};