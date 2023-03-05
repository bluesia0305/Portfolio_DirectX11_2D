#include <Windows.h>
#include "Game.h"

namespace Rendering	{ void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }
namespace Input		{ void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }
namespace Sound		{ void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }
namespace Time		{ void Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }
namespace Test		{ void Update(); }

namespace Engine
{
	namespace { Game* Portfolio; }

	LRESULT CALLBACK Procedure(HWND const hWnd,
							   UINT const uMsg,
							   WPARAM const wParam,
							   LPARAM const lParam)
	{
		switch (uMsg)
		{
		case WM_APP:
		{
			if (!Portfolio->Update()) CloseWindow(hWnd);
			Input::Procedure(hWnd, uMsg, wParam, lParam);
			Rendering::Procedure(hWnd, uMsg, wParam, lParam);
			Time::Procedure(hWnd, uMsg, wParam, lParam);
			return 0;
		}
		
		case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL: case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN: case WM_XBUTTONUP: case WM_XBUTTONDBLCLK:
		case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		case WM_KEYDOWN: case WM_KEYUP:
		{
			Input::Procedure(hWnd, uMsg, wParam, lParam);
			return 0;
		}

		case WM_CREATE:
		{
			Rendering::Procedure(hWnd, uMsg, wParam, lParam);
			Sound::Procedure(hWnd, uMsg, wParam, lParam);
			(Portfolio = Initialize())->Start();
			return 0;
		}

		case WM_SIZE:
		{
			Rendering::Procedure(hWnd, uMsg, wParam, lParam);
			return 0;
		}

		case WM_DESTROY:
		{
			Portfolio->End();
			delete Portfolio;

			Rendering::Procedure(hWnd, uMsg, wParam, lParam);
			Sound::Procedure(hWnd, uMsg, wParam, lParam);
			PostQuitMessage(0);
			return 0;
		}

		default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}