#include "Mesh.h"

Mesh::~Mesh()
{
    if (m_vertexBuffer) m_vertexBuffer->Release();
    if (m_indexBuffer) m_indexBuffer->Release();
}

bool Mesh::CreateCube(ID3D11Device* device)
{
    std::vector<Vertex> vertices =
    {
        // Front
        {{-1,-1,-1}, { 0, 0,-1}, {1,0,0}},
        {{-1, 1,-1}, { 0, 0,-1}, {0,1,0}},
        {{ 1, 1,-1}, { 0, 0,-1}, {0,0,1}},
        {{ 1,-1,-1}, { 0, 0,-1}, {1,1,0}},

        // Back
        {{ 1,-1, 1}, { 0, 0, 1}, {1,0,1}},
        {{ 1, 1, 1}, { 0, 0, 1}, {0,1,1}},
        {{-1, 1, 1}, { 0, 0, 1}, {1,1,1}},
        {{-1,-1, 1}, { 0, 0, 1}, {0.2f,0.2f,0.2f}},

        // Left
        {{-1,-1, 1}, {-1, 0, 0}, {1,0,0}},
        {{-1, 1, 1}, {-1, 0, 0}, {0,1,0}},
        {{-1, 1,-1}, {-1, 0, 0}, {0,0,1}},
        {{-1,-1,-1}, {-1, 0, 0}, {1,1,0}},

        // Right
        {{ 1,-1,-1}, { 1, 0, 0}, {1,0,1}},
        {{ 1, 1,-1}, { 1, 0, 0}, {0,1,1}},
        {{ 1, 1, 1}, { 1, 0, 0}, {1,1,1}},
        {{ 1,-1, 1}, { 1, 0, 0}, {0.3f,0.3f,0.3f}},

        // Top
        {{-1, 1,-1}, { 0, 1, 0}, {1,0,0}},
        {{-1, 1, 1}, { 0, 1, 0}, {0,1,0}},
        {{ 1, 1, 1}, { 0, 1, 0}, {0,0,1}},
        {{ 1, 1,-1}, { 0, 1, 0}, {1,1,0}},

        // Bottom
        {{-1,-1, 1}, { 0,-1, 0}, {1,0,1}},
        {{-1,-1,-1}, { 0,-1, 0}, {0,1,1}},
        {{ 1,-1,-1}, { 0,-1, 0}, {1,1,1}},
        {{ 1,-1, 1}, { 0,-1, 0}, {0.4f,0.4f,0.4f}},
    };

    std::vector<UINT> indices =
    {
         0, 1, 2,  0, 2, 3,   // Front
         4, 5, 6,  4, 6, 7,   // Back
         8, 9,10,  8,10,11,   // Left
        12,13,14, 12,14,15,   // Right
        16,17,18, 16,18,19,   // Top
        20,21,22, 20,22,23    // Bottom
    };

    m_indexCount = static_cast<UINT>(indices.size());

    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vsd = {};
    vsd.pSysMem = vertices.data();

    if (FAILED(device->CreateBuffer(&vbd, &vsd, &m_vertexBuffer)))
        return false;

    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices.data();

    if (FAILED(device->CreateBuffer(&ibd, &isd, &m_indexBuffer)))
        return false;

    return true;
}

void Mesh::Bind(ID3D11DeviceContext* context) const
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::Draw(ID3D11DeviceContext* context) const
{
    context->DrawIndexed(m_indexCount, 0, 0);
}