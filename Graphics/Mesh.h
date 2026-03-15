#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 color;
};

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    bool CreateCube(ID3D11Device* device);
    void Bind(ID3D11DeviceContext* context) const;
    void Draw(ID3D11DeviceContext* context) const;

private:
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;
    UINT m_indexCount = 0;
};