#pragma once
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include <d3d11.h>
#include <DirectXMath.h>

class Renderer
{
public:
    explicit Renderer(Window& window);
    ~Renderer();

    bool Initialize();
    void Update(float dt);
    void Render();
    bool Resize(int width, int height);

private:
    bool CreateDeviceAndSwapChain();
    bool CreateRenderTarget();
    bool CreateDepthBuffer();
    bool CreateConstantBuffers();
    void SetViewport();

private:
    struct ObjectCB
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 projection;
    };

    struct SceneCB
    {
        DirectX::XMFLOAT3 cameraPosition;
        float ambientStrength = 0.15f;

        DirectionalLight directionalLight;
        PointLight pointLight;
    };

private:
    Window& m_window;

    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;
    ID3D11RenderTargetView* m_rtv = nullptr;
    ID3D11Texture2D* m_depthBuffer = nullptr;
    ID3D11DepthStencilView* m_dsv = nullptr;
    ID3D11Buffer* m_objectCB = nullptr;
    ID3D11Buffer* m_sceneCB = nullptr;

    Mesh m_cube;
    Shader m_shader;
    Camera m_camera;
    Transform m_cubeTransform;

    DirectionalLight m_dirLight;
    PointLight m_pointLight;

    float m_time = 0.0f;

    float m_fpsTimer = 0.0f;
    int m_fpsFrameCount = 0;
    float m_currentFps = 0.0f;
};