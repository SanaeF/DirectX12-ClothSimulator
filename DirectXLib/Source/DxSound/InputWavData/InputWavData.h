#pragma once
#pragma comment(lib, "winmm.lib")
#include <dsound.h>
namespace lib {
	class InputWavData {
	private:
		struct WavData{
			WAVEFORMATEX WavFormat;
			char* SoundBuffer;
			DWORD Size;
		};
		WavData mWaveData;
		LONG read_size;
		HMMIO mmio_handle = NULL;
		MMCKINFO ck_info;
		MMCKINFO riffck_info;
	public:
		int loadFile(const char* file_name);
		WavData getWavData();
	private:
		bool mWavProp();
		bool mFmtProp();
		bool mDataProp();
		bool mLoadSound();
	};
}