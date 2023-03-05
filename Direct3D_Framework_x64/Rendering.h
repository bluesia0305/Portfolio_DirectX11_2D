#pragma once
#define _USE_MATH_DEFINES

#include <Windows.h>
#include <string>
#include <map>

#include "FreeImage.h"
#include "Pipeline.h"
#include "Resource.h"
#include "Input.h"
#include "Time.h"
#include "vector.h"

namespace Rendering
{
	class Camera final
	{
	public:
		vector<2>			Position;
		vector<2>			ProjScale;
		vector<3>			Angle;
		vector<2>			ZoomPos;
		vector<2>			ZoomScale;

		Camera();
		void Set() const;
	};

	namespace Image
	{
		class Component final
		{
		public:
			const char*		Content;
			vector<2>		Position;
			vector<2>		Length;
			vector<3>		Angle;
			float			AlphaLevel;
			
			float			RevRadius;
			vector<3>		RevAngle;

			void Draw();
		};

		void Import(std::string const&);
	}

	namespace Animation
	{
		class Component final
		{
		public:
			const char*		Content;
			vector<2>		Position;
			vector<2>		Length;
			vector<3>		Angle;
			float			AlphaLevel;

			float			Playback;
			float			Duration;
			bool			Loop;
			bool			End;

			float			RevRadius;
			vector<3>		RevAngle;

			void Draw();
			__forceinline void Repeat();
		};

		void Import(std::string const&);
	}

	namespace Background
	{
		class Component final
		{
		public:
			const char*		Content;
			vector<2>		Position;
			vector<2>		Length;
			vector<3>		Angle;
			float			AlphaLevel;
			
			vector<2>		ZoomPos;
			vector<2>		ZoomScale;

			void Draw();
		};

		void Import(std::string const&);
	}

	namespace Text
	{
		class Component final
		{
		public:
			const char*		String;
			vector<2>		Position;

			struct
			{
				const char*		Name;
				unsigned		Size;
				bool			bBold;
				bool			bItalic;
				bool			bUnderlined;
				bool			bStrikeThrough;
			}Font;

			struct
			{
				unsigned char	Red;
				unsigned char	Green;
				unsigned char	Blue;
			}Color;

			void Draw();
		};

		void Import(std::string const&);
	}

	void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
}