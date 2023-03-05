#pragma once
#pragma comment(lib, "xaudio2.lib")
#include <xaudio2.h>
#include <Windows.h>
#include <cassert>
#include <string>
#include <map>
#include "Resource.h"

#ifdef NDEBUG
#define MUST(some_expr) (some_expr)
#else
#define MUST(some_expr) (assert(SUCCEEDED(some_expr)))
#endif

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

namespace Sound
{
	struct Handle final
	{
		IXAudio2SourceVoice*	SourceVoice;
		XAUDIO2_BUFFER			Buffer;
	};

	class Sound final
	{
	public:
		const char* Content;
		float		Volume;
		bool		Loop;
		bool		isPause;

		void Play();
		void Stop();
		void Pause();
		void SetVolume(float);
		void VolumeUp();
		void VolumeDown();
		void GradualVolumeUp(float);
		void GradualVolumeDown(float);
	};

	HRESULT FindAudio(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadAudioData(HANDLE, void*, DWORD, DWORD);
	void Import(std::string const&);
	void Procedure(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam);
}