#include "Renderer.h"
#include <cstring>
#include <string>
#include <sstream>

using namespace DirectX;

Renderer::Renderer(Window& window)
    : m_window(window)
{
}

Renderer::~Renderer()
{
    if (m_sceneCB) m_sceneCB->Release();
    if (m_objectCB) m_objectCB->Release();
    if (m_dsv) m_dsv->Release();
    if (m_depthBuffer) m_depthBuffer->Release();
    if (m_rtv) m_rtv->Release();
    if (m_swapChain) m_swapChain->Release();
    if (m_context) m_context->Release();
    if (m_device) m_device->Release();
}

bool Renderer::Initialize()
{
    if (!CreateDeviceAndSwapChain()) return false;
    if (!CreateRenderTarget()) return false;
    if (!CreateDepthBuffer()) return false;
    if (!CreateConstantBuffers()) return false;

    SetViewport();

    if (!m_cube.CreateCube(m_device)) return false;
    if (!m_shader.Load(m_device, L"BasicVS.hlsl", L"BasicPS.hlsl")) return false;

    m_camera.SetAspect(static_cast<float>(m_window.GetWidth()) / static_cast<float>(m_window.GetHeight()));

    m_cubeTransform.scale = { 1.0f, 1.0f, 1.0f };
    m_cubeTransform.position = { 0.0f, 0.0f, 0.0f };

    m_dirLight.direction = { -0.4f, -1.0f, 0.3f };
    m_dirLight.color = { 1.0f, 1.0f, 1.0f };
    m_dirLight.intensity = 0.9f;

    m_pointLight.position = { 2.5f, 2.0f, -2.0f };
    m_pointLight.color = { 1.0f, 0.85f, 0.65f };
    m_pointLight.intensity = 2.0f;
    m_pointLight.range = 12.0f;

    return true;
}

bool Renderer::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = static_cast<UINT>(m_window.GetWidth());
    scd.BufferDesc.Height = static_cast<UINT>(m_window.GetHeight());
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_window.GetHWND();
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL requested[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createFlags,
        requested,
        ARRAYSIZE(requested),
        D3D11_SDK_VERSION,
        &scd,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_context
    );

    return SUCCEEDED(hr);
}

bool Renderer::CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (FAILED(hr)) return false;

    hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_rtv);
    backBuffer->Release();

    return SUCCEEDED(hr);
}

bool Renderer::CreateDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = static_cast<UINT>(m_window.GetWidth());
    depthDesc.Height = static_cast<UINT>(m_window.GetHeight());
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    HRESULT hr = m_device->CreateTexture2D(&depthDesc, nullptr, &m_depthBuffer);
    if (FAILED(hr)) return false;

    hr = m_device->CreateDepthStencilView(m_depthBuffer, nullptr, &m_dsv);
    if (FAILED(hr)) return false;

    m_context->OMSetRenderTargets(1, &m_rtv, m_dsv);
    return true;
}

bool Renderer::CreateConstantBuffers()
{
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.ByteWidth = sizeof(ObjectCB);

    if (FAILED(m_device->CreateBuffer(&cbd, nullptr, &m_objectCB)))
        return false;

    std::memset(&cbd, 0, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.ByteWidth = sizeof(SceneCB);

    if (FAILED(m_device->CreateBuffer(&cbd, nullptr, &m_sceneCB)))
        return false;

    return true;
}

void Renderer::SetViewport()
{
    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = static_cast<float>(m_window.GetWidth());
    vp.Height = static_cast<float>(m_window.GetHeight());
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    m_context->RSSetViewports(1, &vp);
}

bool Renderer::Resize(int width, int height)
{
    if (!m_swapChain || width <= 0 || height <= 0)
        return false;

    if (m_context)
    {
        m_context->OMSetRenderTargets(0, nullptr, nullptr);
        m_context->ClearState();
    }

    if (m_dsv)
    {
        m_dsv->Release();
        m_dsv = nullptr;
    }

    if (m_depthBuffer)
    {
        m_depthBuffer->Release();
        m_depthBuffer = nullptr;
    }

    if (m_rtv)
    {
        m_rtv->Release();
        m_rtv = nullptr;
    }

    HRESULT hr = m_swapChain->ResizeBuffers(
        0,
        static_cast<UINT>(width),
        static_cast<UINT>(height),
        DXGI_FORMAT_UNKNOWN,
        0
    );

    if (FAILED(hr))
        return false;

    if (!CreateRenderTarget())
        return false;

    if (!CreateDepthBuffer())
        return false;

    SetViewport();

    if (height > 0)
    {
        m_camera.SetAspect(static_cast<float>(width) / static_cast<float>(height));
    }

    return true;
}

void Renderer::Update(float dt)
{
    m_time += dt;

    m_cubeTransform.rotation.y += dt;
    m_cubeTransform.rotation.x += dt * 0.45f;

    m_pointLight.position.x = cosf(m_time) * 3.0f;
    m_pointLight.position.z = sinf(m_time) * 3.0f;
    m_pointLight.position.y = 1.8f + sinf(m_time * 2.0f) * 0.5f;

    // FPS
    m_fpsTimer += dt;
    m_fpsFrameCount++;

    if (m_fpsTimer >= 1.0f)
    {
        m_currentFps = static_cast<float>(m_fpsFrameCount) / m_fpsTimer;

        std::wstringstream ss;
        ss.precision(1);
        ss << std::fixed
            << L"DirectX 11 Mini Engine - Rotating Lit Cube | FPS: "
            << m_currentFps
            << L" | Size: "
            << m_window.GetWidth()
            << L"x"
            << m_window.GetHeight();

        m_window.SetTitle(ss.str());

        m_fpsTimer = 0.0f;
        m_fpsFrameCount = 0;
    }
}

void Renderer::Render()
{
    const float clearColor[4] = { 0.08f, 0.10f, 0.15f, 1.0f };
    m_context->ClearRenderTargetView(m_rtv, clearColor);
    m_context->ClearDepthStencilView(m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_shader.Bind(m_context);
    m_cube.Bind(m_context);

    XMMATRIX world = m_cubeTransform.GetMatrix();
    XMMATRIX view = m_camera.GetView();
    XMMATRIX proj = m_camera.GetProjection();

    ObjectCB objCB = {};
    XMStoreFloat4x4(&objCB.world, XMMatrixTranspose(world));
    XMStoreFloat4x4(&objCB.view, XMMatrixTranspose(view));
    XMStoreFloat4x4(&objCB.projection, XMMatrixTranspose(proj));

    m_context->UpdateSubresource(m_objectCB, 0, nullptr, &objCB, 0, 0);

    SceneCB sceneCB = {};
    sceneCB.cameraPosition = XMFLOAT3(0.0f, 1.5f, -6.0f);
    sceneCB.ambientStrength = 0.18f;
    sceneCB.directionalLight = m_dirLight;
    sceneCB.pointLight = m_pointLight;

    m_context->UpdateSubresource(m_sceneCB, 0, nullptr, &sceneCB, 0, 0);

    m_context->VSSetConstantBuffers(0, 1, &m_objectCB);
    m_context->PSSetConstantBuffers(1, 1, &m_sceneCB);

    m_cube.Draw(m_context);

    m_swapChain->Present(1, 0);
}