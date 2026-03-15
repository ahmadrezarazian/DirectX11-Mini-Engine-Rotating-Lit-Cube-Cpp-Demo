#pragma once
#include <DirectXMath.h>

class Camera
{
public:
    Camera()
    {
        m_position = { 0.0f, 1.5f, -6.0f };
        m_target = { 0.0f, 0.0f, 0.0f };
        m_up = { 0.0f, 1.0f, 0.0f };
        m_fovY = DirectX::XM_PIDIV4;
        m_aspect = 16.0f / 9.0f;
        m_nearZ = 0.1f;
        m_farZ = 100.0f;
    }

    void SetAspect(float aspect) { m_aspect = aspect; }

    DirectX::XMMATRIX GetView() const
    {
        using namespace DirectX;
        return XMMatrixLookAtLH(
            XMLoadFloat3(&m_position),
            XMLoadFloat3(&m_target),
            XMLoadFloat3(&m_up)
        );
    }

    DirectX::XMMATRIX GetProjection() const
    {
        return DirectX::XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
    }

private:
    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT3 m_target;
    DirectX::XMFLOAT3 m_up;

    float m_fovY;
    float m_aspect;
    float m_nearZ;
    float m_farZ;
};