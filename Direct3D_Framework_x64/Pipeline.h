#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <cassert>

#ifdef NDEBUG
#define MUST(some_expr) (some_expr)
#else
#define MUST(some_expr) (assert(SUCCEEDED(some_expr)))
#endif

namespace Pipeline
{
	namespace
	{
		namespace Buffer
		{
			template<typename T>
			void Update(ID3D11Buffer*, T const&);
		}
	}

	namespace Transform
	{
		enum class MATRIX_TYPE
		{
			World,
			ViewByProj
		};

		using MATRIX = float[4][4];
		template<MATRIX_TYPE type> void Update(MATRIX const&);
		template void Update<MATRIX_TYPE::World>(MATRIX const& matrix);
		template void Update<MATRIX_TYPE::ViewByProj>(MATRIX const& matrix);
	}

	namespace Transparency
	{
		using VECTOR = float[4];
		void Update(VECTOR const&);
	}

	namespace Texture
	{
		struct Handle final { ID3D11ShaderResourceView* ShaderResourceView; };
		void Create(struct Handle*&, SIZE const, BYTE const* const);
		void Render(Handle const* const&, RECT const, float);
		void Delete(Handle const* const&);
	}
	
	namespace Text
	{
		void Render(HFONT const, LPCSTR const, COLORREF const, POINT const, SIZE const);
	}

	void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}