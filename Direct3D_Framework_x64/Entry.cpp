#include <Windows.h>
#include <iostream>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#define	CXSCREEN 1280
#define CYSCREEN 720

namespace Engine { LRESULT CALLBACK Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }

int APIENTRY WinMain(_In_ HINSTANCE const hInstance,
					 _In_opt_ HINSTANCE const hPrevInstance,
					 _In_ LPSTR lpCmdLine,
					 _In_ int const nCmdShow)
{
	HWND hWnd = nullptr;

	{
		WNDCLASSEX WndClass
		{
			sizeof(WNDCLASSEX),
			CS_VREDRAW | CS_HREDRAW,
			Engine::Procedure,
			NULL,
			NULL,
			hInstance,
			LoadIcon(nullptr, IDI_APPLICATION),
			LoadCursor(nullptr, IDC_ARROW),
			static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
			nullptr,
			"D3D11_Game",
			LoadIcon(nullptr, IDI_APPLICATION),
		};

		RegisterClassEx(&WndClass);
	}
	
	{
		CREATESTRUCT window
		{
			nullptr,
			hInstance,
			nullptr,
			nullptr,
			0,
			0,
			0,
			0,
			WS_OVERLAPPEDWINDOW,
			"Portfolio",
			"D3D11_Game",
			NULL
		};
		
		RECT rect{ 0, 0, CXSCREEN, CYSCREEN };
		AdjustWindowRectEx(&rect, window.style, static_cast<bool>(window.hMenu), window.dwExStyle);

		window.cx = rect.right - rect.left;
		window.cy = rect.bottom - rect.top;
		window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2;
		window.y = (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2;

		hWnd = FindWindow("D3D11_Game", "Portfolio");
		if (!hWnd)
		{
			hWnd = CreateWindowEx(window.dwExStyle,
								  window.lpszClass,
								  window.lpszName,
								  window.style,
								  window.x,
								  window.y,
								  window.cx,
								  window.cy,
								  window.hwndParent,
								  window.hMenu,
								  hInstance,
								  window.lpCreateParams);
			ShowWindow(hWnd, nCmdShow);
		}
		else SetForegroundWindow(FindWindow("D3D11_Game", "Portfolio"));
	}

	{
		MSG msg = MSG();
		while (true)
		{
			if (PeekMessage(&msg, HWND(), NULL, NULL, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) return static_cast<int>(msg.wParam);
				else DispatchMessage(&msg);
			}
			else
			{ SendMessage(hWnd, WM_APP, msg.wParam, msg.lParam); }
		}
	}

	return 0;
}