#pragma once
#pragma comment(lib, "winmm.lib")
#include <dsound.h>

class DxWavData {
private:

	struct WavData
	{
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

	int LoadFile(const char* file_name);

	WavData getWavData() {
		return mWaveData;
	}

private:

	bool mWavProp();

	bool mFmtProp();

	bool mDataProp();

	bool mLoadSound();

};