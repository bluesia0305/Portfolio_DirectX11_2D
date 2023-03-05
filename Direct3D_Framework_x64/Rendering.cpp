#include "Rendering.h"

namespace Rendering
{
	namespace Matrix
	{
		inline matrix<4, 4> Scaling(vector<2> scale)
		{
			return matrix<4, 4>
			{
				scale[0],		0,				0,				0,
				0,				scale[1],		0,				0,
				0,				0,				1,				0,
				0,				0,				0,				1
			};
		}

		inline matrix<4, 4> Rotation(vector<3> angle)
		{
			vector<3> const rad = { static_cast<float>(M_PI) / 180.0f * angle[0],
									static_cast<float>(M_PI) / 180.0f * angle[1],
									static_cast<float>(M_PI) / 180.0f * angle[2] };

			matrix<4, 4> const rotX =
			{
				1,				0,				0,				0,
				0,				cos(rad[0]),	-sin(rad[1]),	0,
				0,				sin(rad[0]),	cos(rad[1]),	0,
				0,				0,				0,				1
			};

			matrix<4, 4> const rotY =
			{
				cos(rad[1]),	0,				sin(rad[1]),	0,
				0,				1,				0,				0,
				-sin(rad[1]),	0,				cos(rad[1]),	0,
				0,				0,				0,				1
			};

			matrix<4, 4> const rotZ =
			{
				cos(rad[2]),	-sin(rad[2]),	0,				0,
				sin(rad[2]),	cos(rad[2]),	0,				0,
				0,				0,				1,				0,
				0,				0,				0,				1
			};
			
			return rotZ * rotY * rotX;
		}

		inline matrix<4, 4> Translation(vector<2> displacement)
		{
			return matrix<4, 4>
			{
				1,				0,				0,				displacement[0],
				0,				1,				0,				displacement[1],
				0,				0,				1,				0,
				0,				0,				0,				1
			};
		}

		inline matrix<4, 4> Revolution(vector<2> revCenter, float revRadius, vector<3> revAngle)
		{
			vector<3> const rad = { static_cast<float>(M_PI) / 180.0f * revAngle[0],
									static_cast<float>(M_PI) / 180.0f * revAngle[1],
									static_cast<float>(M_PI) / 180.0f * revAngle[2] };

			return Translation(revCenter) * Rotation(revAngle) * Translation(vector<2>{ revRadius, 0 }) * Translation(-revCenter);
		}

		inline matrix<4, 4> Zoom(vector<2> zoomPos, vector<2> zoomScale)
		{
			return Translation(zoomPos) * Scaling(zoomScale) * Translation(-zoomPos);
		}
	}

	Camera::Camera()
	{
		Position	 = vector<2>{ 0, 0 };
		ProjScale	 = vector<2>{ 2.0f / 1280.0f, 2.0f / 720.0f };
		Angle		 = vector<3>{ 0, 0, 0 };
		ZoomPos		 = vector<2>{ 0, 0 };
		ZoomScale	 = vector<2>{ 1, 1 };
	}

	void Camera::Set() const
	{
		matrix<4, 4> const view		  = Rendering::Matrix::Translation(-Position) * Rendering::Matrix::Rotation(-Angle);
		matrix<4, 4> const projection = Rendering::Matrix::Scaling(ProjScale);
		matrix<4, 4> camera			  = projection * view * Rendering::Matrix::Zoom(ZoomPos, ZoomScale);
		Pipeline::Transform::Update<Pipeline::Transform::MATRIX_TYPE::ViewByProj>(reinterpret_cast<Pipeline::Transform::MATRIX const&>(camera));
	}

	namespace Image
	{
		struct DESC final
		{
		public:
			Pipeline::Texture::Handle*	Handle;
			SIZE						Size;
		};

		std::map<std::string, DESC> Storage;

		void Component::Draw()
		{
			using namespace Matrix;

			matrix<4, 4> const world = Revolution(Position, RevRadius, RevAngle) * Translation(Position) * Rotation(Angle) * Scaling(Length);
			Pipeline::Transform::Update<Pipeline::Transform::MATRIX_TYPE::World>(reinterpret_cast<Pipeline::Transform::MATRIX const&>(world));

			DESC const& image = Storage.at(Content);
			Pipeline::Texture::Render(image.Handle, { 0, 0, image.Size.cx, image.Size.cy }, AlphaLevel);
		}

		void Import(std::string const& FilePath)
		{
			FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(FilePath.data()), FilePath.data());
			FreeImage_FlipVertical(bitmap);
			if (FreeImage_GetBPP(bitmap) != 32)
			{
				FIBITMAP* const prevBitmap = bitmap;
				bitmap = FreeImage_ConvertTo32Bits(bitmap);
				FreeImage_Unload(prevBitmap);
			}
			
			DESC descriptor = DESC();
			{
				descriptor.Size.cx = FreeImage_GetWidth(bitmap);
				descriptor.Size.cy = FreeImage_GetHeight(bitmap);
				Pipeline::Texture::Create(descriptor.Handle, descriptor.Size, FreeImage_GetBits(bitmap));
			}

			{
				std::string filePath   = FilePath;
				std::replace(filePath.begin(), filePath.end(), '/', '\\');
				size_t const str_start = filePath.find_last_of('\\') + sizeof(char);
				size_t const str_end   = filePath.find_first_of('.');
				Storage.try_emplace(filePath.substr(str_start, str_end - str_start), descriptor);
			}
			FreeImage_Unload(bitmap);
		}
	}

	namespace Animation
	{
		struct DESC final
		{
		public:
			Pipeline::Texture::Handle*	Handle;
			SIZE						Frame;
			UINT						Motion;
		};

		std::map<std::string, DESC> Storage;

		void Component::Draw()
		{
			using namespace Matrix;

			matrix<4, 4> const world = Revolution(Position, RevRadius, RevAngle) * Translation(Position) * Rotation(Angle) * Scaling(Length);
			Pipeline::Transform::Update<Pipeline::Transform::MATRIX_TYPE::World>(reinterpret_cast<Pipeline::Transform::MATRIX const&>(world));

			DESC const& animation = Storage.at(Content);
			LONG const progress = static_cast<LONG>((Playback / Duration) * animation.Motion);
			RECT const area
			{
				animation.Frame.cx * (progress + 0), (animation.Frame.cy * 0),
				animation.Frame.cx * (progress + 1), (animation.Frame.cy * 1)
			};

			Pipeline::Texture::Render(animation.Handle, area, AlphaLevel);
			
			float const delta = Time::Get::Delta();
			Playback += delta;
			if (Duration <= Playback)
			{
				if (Loop) Playback = Duration / 0x10000 * (static_cast<int>(Playback / Duration * 0x10000) & 0xffff);
				else
				{
					Playback -= delta;
					End = true;
				}

			}
		}

		__forceinline void Component::Repeat()
		{
			End = false;
			Playback = 0.f;
		}

		void Import(std::string const& FilePath)
		{
			FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(FilePath.data()), FilePath.data());
			FreeImage_FlipVertical(bitmap);
			if (FreeImage_GetBPP(bitmap) != 32)
			{
				FIBITMAP* const prevBitmap = bitmap;
				bitmap = FreeImage_ConvertTo32Bits(bitmap);
				FreeImage_Unload(prevBitmap);
			}

			DESC descriptor = DESC();
			{
				descriptor.Frame.cx = FreeImage_GetWidth(bitmap);
				descriptor.Frame.cy = FreeImage_GetHeight(bitmap);
				Pipeline::Texture::Create(descriptor.Handle, descriptor.Frame, FreeImage_GetBits(bitmap));
			}

			{
				std::string filePath	  = FilePath;
				std::replace(filePath.begin(), filePath.end(), '/', '\\');
				size_t const name_start	  = filePath.find_last_of('\\') + sizeof(char);
				size_t const motion_start = filePath.find_last_of('[');
				size_t const motion_end	  = filePath.find_last_of(']');
				descriptor.Motion		  = atoi(filePath.substr(motion_start + sizeof(char), motion_end - (motion_start + sizeof(char))).data());
				descriptor.Frame.cx		 /= descriptor.Motion;
				Storage.try_emplace(filePath.substr(name_start, motion_start - name_start), descriptor);
			}
			FreeImage_Unload(bitmap);
		}
	}

	namespace Background
	{
		struct DESC final
		{
		public:
			Pipeline::Texture::Handle*	Handle;
			SIZE						Size;
		};

		std::map<std::string, DESC> Storage;

		void Component::Draw()
		{
			using namespace Matrix;

			matrix<4, 4> const world = Zoom(ZoomPos, ZoomScale) * Translation(Position) * Rotation(Angle) * Scaling(Length);
			Pipeline::Transform::Update<Pipeline::Transform::MATRIX_TYPE::World>(reinterpret_cast<Pipeline::Transform::MATRIX const&>(world));

			DESC const& image = Storage.at(Content);
			Pipeline::Texture::Render(image.Handle, { 0, 0, image.Size.cx, image.Size.cy }, AlphaLevel);
		}

		void Import(std::string const& FilePath)
		{
			FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(FilePath.data()), FilePath.data());
			FreeImage_FlipVertical(bitmap);
			if (FreeImage_GetBPP(bitmap) != 32)
			{
				FIBITMAP* const prevBitmap = bitmap;
				bitmap = FreeImage_ConvertTo32Bits(bitmap);
				FreeImage_Unload(prevBitmap);
			}
			
			DESC descriptor = DESC();
			{
				descriptor.Size.cx = FreeImage_GetWidth(bitmap);
				descriptor.Size.cy = FreeImage_GetHeight(bitmap);
				Pipeline::Texture::Create(descriptor.Handle, descriptor.Size, FreeImage_GetBits(bitmap));
			}

			{
				std::string filePath = FilePath;
				std::replace(filePath.begin(), filePath.end(), '/', '\\');
				size_t const str_start = filePath.find_last_of('\\') + sizeof(char);
				size_t const str_end   = filePath.find_first_of('.');
				Storage.try_emplace(filePath.substr(str_start, str_end - str_start), descriptor);
			}
			FreeImage_Unload(bitmap);
		}
	}

	namespace Text
	{
		void Component::Draw()
		{
			LOGFONT descriptor	   = LOGFONT();
			descriptor.lfHeight	   = Font.Size;
			descriptor.lfWeight	   = Font.bBold ? FW_BOLD : FW_NORMAL;
			descriptor.lfItalic	   = Font.bItalic;
			descriptor.lfUnderline = Font.bUnderlined;
			descriptor.lfStrikeOut = Font.bStrikeThrough;
			descriptor.lfCharSet   = DEFAULT_CHARSET;
			strcpy_s(descriptor.lfFaceName, LF_FACESIZE, Font.Name);

			HFONT const font	   = CreateFontIndirect(&descriptor);
			POINT const	center	   = { static_cast<LONG>(Position[0]), static_cast<LONG>(Position[1]) };
			SIZE const	area	   = { static_cast<LONG>(Font.Size * (strlen(String) / 2)), static_cast<LONG>(Font.Size) };
			Pipeline::Text::Render(font, String, RGB(Color.Red, Color.Green, Color.Blue), center, area);
			DeleteObject(font);
		}

		void Import(std::string const& file)
		{ AddFontResourceEx(file.data(), FR_PRIVATE | FR_NOT_ENUM, 0); }
	}

	void Procedure(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			Pipeline::Procedure(hWnd, uMsg, wParam, lParam);
			Resource::Import("Resources/Fonts", Text::Import);
			FreeImage_Initialise();
			{
				Resource::Import("Resources/Images", Image::Import);
				Resource::Import("Resources/Animations", Animation::Import);
				Resource::Import("Resources/Backgrounds", Background::Import);
			}
			FreeImage_DeInitialise();
			return;
		}

		case WM_SIZE:
		{
			Pipeline::Procedure(hWnd, uMsg, wParam, lParam);
			return;
		}

		case WM_APP:
		{
			Pipeline::Procedure(hWnd, uMsg, wParam, lParam);
			return;
		}

		case WM_DESTROY:
		{
			Pipeline::Procedure(hWnd, uMsg, wParam, lParam);
			for (auto const& pair : Image::Storage)		Pipeline::Texture::Delete(pair.second.Handle);
			for (auto const& pair : Animation::Storage)	Pipeline::Texture::Delete(pair.second.Handle);
			return;
		}
		}
	}
}