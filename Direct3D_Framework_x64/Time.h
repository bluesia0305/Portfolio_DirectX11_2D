#pragma once
#include <chrono>
#include <Windows.h>

namespace Time
{
	namespace Get
	{
		[[nodiscard]] float Elapsed();
		[[nodiscard]] float Delta();
	}

	void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}