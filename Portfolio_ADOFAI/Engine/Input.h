#pragma once
#include <Windows.h>

namespace Input
{
	namespace Get
	{
		namespace Key
		{
			[[nodiscard]] bool Down(WPARAM const);
			[[nodiscard]] bool Pressed(WPARAM const);
			[[nodiscard]] bool Up(WPARAM const);
		}

		namespace Cursor
		{
			[[nodiscard]] long X();
			[[nodiscard]] long Y();
		}

		namespace Wheel
		{
			[[nodiscard]] long H();
			[[nodiscard]] long V();
		}
	}

	void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}