#pragma once
#include <windows.h>
#include <string>

class Window
{
public:
    Window(HINSTANCE hInstance, int width, int height, const std::wstring& title);
    ~Window();

    bool Create(int nCmdShow);
    bool ProcessMessages();

    HWND GetHWND() const { return m_hwnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    bool WasResized();
    void SetTitle(const std::wstring& title);

private:
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE m_hInstance = nullptr;
    HWND m_hwnd = nullptr;
    int m_width = 1280;
    int m_height = 720;
    std::wstring m_title;
    std::wstring m_className = L"DX11MiniEngineWindowClass";

    bool m_resized = false;
};