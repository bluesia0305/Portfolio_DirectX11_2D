#include "Input.h"
#include <iostream>

namespace Input
{
	class
	{
	public:
		void Down(WPARAM const code)
		{
			if (!Pressed(code))
			{
				BitSwitch.Pressed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
				BitSwitch.Changed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
			}
		}
		void Up(WPARAM const code)
		{
			BitSwitch.Pressed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
			BitSwitch.Changed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
		}
		bool Pressed(WPARAM const code)	{ return BitSwitch.Pressed[code >> 0x4] & (0x8000 >> (code & 0xf)); }
		bool Changed(WPARAM const code) { return BitSwitch.Changed[code >> 0x4] & (0x8000 >> (code & 0xf)); }
		void Update()					{ ZeroMemory(BitSwitch.Changed, sizeof(BitSwitch.Changed)); }

		struct
		{
			USHORT Pressed[16];
			USHORT Changed[16];
		}BitSwitch;
	}Key;

	POINT Cursor;
	POINT Wheel;

	namespace Get
	{
		namespace Key
		{
			bool Down(WPARAM const code)	{ return Input::Key.Pressed(code) and Input::Key.Changed(code); }
			bool Pressed(WPARAM const code) { return Input::Key.Pressed(code); }
			bool Up(WPARAM const code)		{ return not Input::Key.Pressed(code) and Input::Key.Changed(code); }
		}

		namespace Cursor
		{
			long X() { return Input::Cursor.x; }
			long Y() { return Input::Cursor.y; }
		}

		namespace Wheel
		{
			long H() { return Input::Wheel.x; }
			long V() { return Input::Wheel.y; }
		}
	}

	void Procedure(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam)
	{
		switch (uMsg)
		{
		// Keyboard 입력
		case WM_KEYDOWN		 : { Key.Down(wParam); return; }
		case WM_KEYUP		 : { Key.Up(wParam); return; }
		case WM_SYSKEYDOWN	 : { Key.Down(wParam); return; }
		case WM_SYSKEYUP	 : { Key.Up(wParam); return; }
		
		// Mouse 입력
		case WM_MOUSEWHEEL	 : { Wheel.y += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; return; }
		case WM_MOUSEHWHEEL	 : { Wheel.x += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; return; }
		case WM_MOUSEMOVE:
		{
			Cursor.x =		 static_cast<LONG>(LOWORD(lParam)) - 1280 / 2;
			Cursor.y = -1 * (static_cast<LONG>(HIWORD(lParam)) - 720 / 2);
			return;
		}
		case WM_LBUTTONDOWN	 : { Key.Down(VK_LBUTTON); return; }
		case WM_RBUTTONDOWN	 : { Key.Down(VK_RBUTTON); return; }
		case WM_MBUTTONDOWN	 : { Key.Down(VK_MBUTTON); return; }
		case WM_XBUTTONDOWN	 :
		{
			switch (GET_XBUTTON_WPARAM(wParam))
			{
			case (XBUTTON1): Key.Down(VK_XBUTTON1); return;
			case (XBUTTON2): Key.Down(VK_XBUTTON2); return;
			}
			return;
		}
		case WM_LBUTTONUP	 : { Key.Up(VK_LBUTTON); return; }
		case WM_RBUTTONUP	 : { Key.Up(VK_RBUTTON); return; }
		case WM_MBUTTONUP	 : { Key.Up(VK_MBUTTON); return; }
		case WM_XBUTTONUP	 :
		{
			switch (GET_XBUTTON_WPARAM(wParam))
			{
			case (XBUTTON1): Key.Up(VK_XBUTTON1); return;
			case (XBUTTON2): Key.Up(VK_XBUTTON2); return;
			}
			return;
		}
		//case WM_LBUTTONDBLCLK: { return; }
		//case WM_RBUTTONDBLCLK: { return; }
		//case WM_MBUTTONDBLCLK: { return; }
		//case WM_XBUTTONDBLCLK: { return; }
		
		case WM_APP:
		{
			Key.Update();
			Wheel = POINT();
			return;
		}
		}
	}
}