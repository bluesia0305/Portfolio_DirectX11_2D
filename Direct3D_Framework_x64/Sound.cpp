#include "Sound.h"
#include "Time.h"

namespace Sound
{
	std::map<std::string, Handle*> Storage;

	namespace
	{
		IXAudio2*					XAudio2;
		IXAudio2MasteringVoice*		MasteringVoice;
		WAVEFORMATEXTENSIBLE		wfx;
	}

	void Sound::Play()
	{
		Handle*& sound = Storage.at(Content);
		if (Loop) sound->Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		else sound->Buffer.LoopCount = 0;
		
		if (isPause)
		{
			MUST(sound->SourceVoice->Start());
			isPause = false;
			return;
		}

		MUST(sound->SourceVoice->Stop());
		MUST(sound->SourceVoice->FlushSourceBuffers());
		MUST(sound->SourceVoice->SubmitSourceBuffer(&sound->Buffer));
		MUST(sound->SourceVoice->SetVolume(Volume));
		MUST(sound->SourceVoice->Start());
	}
	void Sound::Stop()
	{
		Handle*& sound = Storage.at(Content);
		MUST(sound->SourceVoice->Stop());
		MUST(sound->SourceVoice->FlushSourceBuffers());
	}
	void Sound::Pause()
	{
		if (isPause) return;
		isPause = true;

		Handle*& sound = Storage.at(Content);
		MUST(sound->SourceVoice->Stop());
	}
	void Sound::SetVolume(float volume)
	{
		Handle*& sound = Storage.at(Content);
		Volume = volume > 1.0f ? 1.0f : volume < 0.0f ? 0.0f : volume;
		MUST(sound->SourceVoice->SetVolume(Volume));
	}
	void Sound::VolumeUp()
	{
		Handle*& sound = Storage.at(Content);
		if (Volume < 1.0f)
		{
			Volume += 0.05f;
			MUST(sound->SourceVoice->SetVolume(Volume));
		}
	}
	void Sound::VolumeDown()
	{
		Handle*& sound = Storage.at(Content);
		if (Volume > 0.0f)
		{
			Volume -= 0.05f;
			MUST(sound->SourceVoice->SetVolume(Volume));
		}
	}
	void Sound::GradualVolumeUp(float speed)
	{
		Handle*& sound = Storage.at(Content);
		if (Volume < 1.0f)
		{
			Volume += speed * Time::Get::Delta();
			MUST(sound->SourceVoice->SetVolume(Volume));
		}
	}
	void Sound::GradualVolumeDown(float speed)
	{
		Handle*& sound = Storage.at(Content);
		if (Volume > 0.0f)
		{
			Volume -= speed * Time::Get::Delta();
			MUST(sound->SourceVoice->SetVolume(Volume));
		}
	}
	

	HRESULT FindAudio(HANDLE hAudio, DWORD fourcc, DWORD & dwAudioSize, DWORD & dwAudioDataPosition)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hAudio, 0, NULL, FILE_BEGIN)) return HRESULT_FROM_WIN32(GetLastError());
		
		DWORD dwAudioType;
		DWORD dwAudioDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;
		
		while (hr == S_OK)
		{
			DWORD dwRead;
			if (0 == ReadFile(hAudio, &dwAudioType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			
			if (0 == ReadFile(hAudio, &dwAudioDataSize, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			
			if (dwAudioType == fourccRIFF)
			{
				dwRIFFDataSize = dwAudioDataSize;
				dwAudioDataSize = 4;
				if (0 == ReadFile(hAudio, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
			}
			else
			{
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hAudio, dwAudioDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
			}
			
			dwOffset += sizeof(DWORD) * 2;
			
			if (dwAudioType == fourcc)
			{
				dwAudioSize = dwAudioDataSize;
				dwAudioDataPosition = dwOffset;
				return S_OK;
			}
			
			dwOffset += dwAudioDataSize;
			
			if (bytesRead >= dwRIFFDataSize) return S_FALSE;
			
		}
		return S_OK;
	}
	
	HRESULT ReadAudioData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}
	
	void Import(std::string const& file)
	{
		HANDLE hFile = CreateFile(file.c_str(),
								  GENERIC_READ,
								  FILE_SHARE_READ,
								  NULL,
								  OPEN_EXISTING,
								  0,
								  NULL);
	
		if (INVALID_HANDLE_VALUE == hFile) return;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) return;
	
		DWORD dwAudioSize;
		DWORD dwAudioPosition;
		MUST(FindAudio(hFile, fourccRIFF, dwAudioSize, dwAudioPosition));
		DWORD filetype;
		MUST(ReadAudioData(hFile, &filetype, sizeof(DWORD), dwAudioPosition));
		if (filetype != fourccWAVE) return;
	
		MUST(FindAudio(hFile, fourccFMT, dwAudioSize, dwAudioPosition));
		MUST(ReadAudioData(hFile, &wfx, dwAudioSize, dwAudioPosition));
	
		MUST(FindAudio(hFile, fourccDATA, dwAudioSize, dwAudioPosition));
		BYTE * pDataBuffer = new BYTE[dwAudioSize];
		MUST(ReadAudioData(hFile, pDataBuffer, dwAudioSize, dwAudioPosition));
	
		Handle * handle = new Handle();
		handle->Buffer.AudioBytes = dwAudioSize;
		handle->Buffer.pAudioData = pDataBuffer;
		handle->Buffer.Flags = XAUDIO2_END_OF_STREAM;
	
		MUST(XAudio2->CreateSourceVoice(&handle->SourceVoice, (WAVEFORMATEX *)&wfx));
	
		{
			size_t const str_start = file.find_last_of('\\') + sizeof(char);
			size_t const str_end   = file.find_first_of('.');
			Storage.try_emplace(file.substr(str_start, str_end - str_start), handle);
		}
	}
	
	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParam, LPARAM const lParam)
	{
		switch (uMessage)
		{
			case WM_CREATE:
			{
				MUST(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));
				MUST(XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR));
				MUST(XAudio2->CreateMasteringVoice(&MasteringVoice));
				Resource::Import("Resources/Sounds", Import);
				return;
			}
			case WM_DESTROY:
			{
	            for (std::pair<std::string, Handle*> elem : Storage) delete elem.second;
	            XAudio2->Release();
				return;
			}
		}
	}
}