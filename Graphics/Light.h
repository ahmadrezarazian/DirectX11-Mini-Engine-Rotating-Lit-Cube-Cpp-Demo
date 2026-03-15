#pragma once
#include <DirectXMath.h>

struct DirectionalLight
{
    DirectX::XMFLOAT3 direction = { 0.4f, -1.0f, 0.2f };
    float intensity = 1.0f;

    DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
    float padding0 = 0.0f;
};

struct PointLight
{
    DirectX::XMFLOAT3 position = { 2.0f, 2.0f, -2.0f };
    float intensity = 1.5f;

    DirectX::XMFLOAT3 color = { 1.0f, 0.9f, 0.7f };
    float range = 10.0f;
};