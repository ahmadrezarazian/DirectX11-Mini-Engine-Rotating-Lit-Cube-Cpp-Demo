#include "Window.h"
#include "Renderer.h"
#include <windows.h>
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    Window window(hInstance, 1280, 720, L"DirectX 11 Mini Engine - Rotating Lit Cube");
    if (!window.Create(nCmdShow))
        return 0;

    Renderer renderer(window);
    if (!renderer.Initialize())
    {
        MessageBox(nullptr, L"Failed to initialize renderer.", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    LARGE_INTEGER freq = {};
    LARGE_INTEGER prev = {};
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&prev);

    while (window.ProcessMessages())
    {
        if (window.WasResized())
        {
            renderer.Resize(window.GetWidth(), window.GetHeight());
        }

        LARGE_INTEGER now = {};
        QueryPerformanceCounter(&now);

        float dt = static_cast<float>(now.QuadPart - prev.QuadPart) / static_cast<float>(freq.QuadPart);
        prev = now;

        renderer.Update(dt);
        renderer.Render();
    }

    return 0;
}