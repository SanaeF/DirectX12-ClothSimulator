#include "DxSound.h"
#include "DxWavData.h"
#include <cassert>
int DxSound::mHandleCount = 0;

int DxSound::LoadFile(const char* file_name) {
	int HandleID = mHandleCount;
	//pDxSoundBuffer.resize(HandleID + 1);
	DxWavData Data;

	if (Data.LoadFile(file_name) == -1)return -1;
	WAVEFORMATEX WavFormat = Data.getWavData().WavFormat;

	ZeroMemory(&mDxS_BuffDesc, sizeof(DSBUFFERDESC));

	mDxS_BuffDesc.dwSize = sizeof(DSBUFFERDESC);
	mDxS_BuffDesc.dwFlags = 
		DSBCAPS_GLOBALFOCUS | 
		DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLVOLUME;
	mDxS_BuffDesc.dwBufferBytes = Data.getWavData().Size;
	mDxS_BuffDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	mDxS_BuffDesc.lpwfxFormat = &WavFormat;

	auto result = pDxS8->CreateSoundBuffer(
		&mDxS_BuffDesc,
		&pDxSoundBuffer[HandleID],
		nullptr
	);
	if (!SUCCEEDED(result)) {
		assert(0 && "CreateSoundBuffer");
	}
	LPVOID buffer;
	DWORD buffer_size;
	if (FAILED(
		pDxSoundBuffer[HandleID]->Lock(
			0, Data.getWavData().Size, &buffer, &buffer_size, nullptr, nullptr, 0
		)
	))
	{
		delete[] Data.getWavData().SoundBuffer;
		return -1;
	}
	auto soundBuff = Data.getWavData().SoundBuffer;
	memcpy(buffer, soundBuff, buffer_size);
	pDxSoundBuffer[HandleID]->Unlock(&buffer, buffer_size, nullptr, 0);

	mHandleCount++;
	return HandleID;
}

void DxSound::Play(int Handle, int Type) {
	DWORD PlayType = 0;
	if (Type == SOUND::eDXSOUND_NORMAL) {
		PlayType = 0;
	}
	if (Type == SOUND::eDXSOUND_LOOP) {
		PlayType = DSBPLAY_LOOPING;
	}
	pDxSoundBuffer[Handle]->Play(0, 0, PlayType);
}

void DxSound::SetVolume(int num, int Handle) {
	pDxSoundBuffer[Handle]->SetVolume(num);
}

void DxSound::Stop(int Handle) {
	pDxSoundBuffer[Handle]->Stop();

}

void DxSound::InitMem(int Handle) {
	pDxSoundBuffer[Handle]->Stop();
	pDxSoundBuffer[Handle]->Release();
	pDxSoundBuffer[Handle] = nullptr;
}

void DxSound::InitMem() {
	for (int num = 0; num < pDxSoundBuffer.size(); num++) {
		InitMem(num);
	}
	if (pDxS8 != nullptr)
	{
		pDxS8->Release();
		pDxS8 = NULL;
	}
}

DxSound::DxSound(HWND hwnd) :
	mHwnd(hwnd) {
	auto result = DirectSoundCreate8(nullptr, &pDxS8, nullptr);
	result = pDxS8->SetCooperativeLevel(mHwnd, DSSCL_NORMAL);
}

DxSound::~DxSound() {
	InitMem();
}