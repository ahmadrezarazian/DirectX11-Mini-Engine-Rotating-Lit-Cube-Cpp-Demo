#pragma once
#include <DirectXMath.h>

struct Transform
{
    DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

    DirectX::XMMATRIX GetMatrix() const
    {
        using namespace DirectX;

        XMMATRIX t = XMMatrixTranslation(position.x, position.y, position.z);
        XMMATRIX rx = XMMatrixRotationX(rotation.x);
        XMMATRIX ry = XMMatrixRotationY(rotation.y);
        XMMATRIX rz = XMMatrixRotationZ(rotation.z);
        XMMATRIX s = XMMatrixScaling(scale.x, scale.y, scale.z);

        return s * rz * ry * rx * t;
    }
};