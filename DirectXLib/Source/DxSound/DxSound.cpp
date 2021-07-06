#include "DxSound.h"
#include "InputWavData/InputWavData.h"
#include <cassert>
namespace lib {
	int DxSound::m_Handle_count = 0;
	//Load Wav File
	int DxSound::loadFile(const char* file_name) {
		int HandleID = m_Handle_count;
		//pDxSoundBuffer.resize(HandleID + 1);
		InputWavData Data;

		if (Data.loadFile(file_name) == -1)return -1;
		WAVEFORMATEX WavFormat = Data.getWavData().WavFormat;

		ZeroMemory(&m_Dxs_buff_desc, sizeof(DSBUFFERDESC));

		m_Dxs_buff_desc.dwSize = sizeof(DSBUFFERDESC);
		m_Dxs_buff_desc.dwFlags =
			DSBCAPS_GLOBALFOCUS |
			DSBCAPS_CTRLPAN |
			DSBCAPS_CTRLVOLUME;
		m_Dxs_buff_desc.dwBufferBytes = Data.getWavData().Size;
		m_Dxs_buff_desc.guid3DAlgorithm = DS3DALG_DEFAULT;
		m_Dxs_buff_desc.lpwfxFormat = &WavFormat;

		auto result = m_pDx_s8->CreateSoundBuffer(
			&m_Dxs_buff_desc,
			&m_pDx_sound_buffer[HandleID],
			nullptr
		);
		if (!SUCCEEDED(result)) {
			assert(0 && "CreateSoundBuffer");
		}
		LPVOID buffer;
		DWORD buffer_size;
		if (FAILED(
			m_pDx_sound_buffer[HandleID]->Lock(
				0, Data.getWavData().Size, &buffer, &buffer_size, nullptr, nullptr, 0
			)
		))
		{
			delete[] Data.getWavData().SoundBuffer;
			return -1;
		}
		auto soundBuff = Data.getWavData().SoundBuffer;
		memcpy(buffer, soundBuff, buffer_size);
		m_pDx_sound_buffer[HandleID]->Unlock(&buffer, buffer_size, nullptr, 0);

		m_Handle_count++;
		return HandleID;
	}
	//Play Sound
	void DxSound::play(int Handle, int Type) {
		DWORD PlayType = 0;
		if (Type == SOUND_PATTERN::DX_SOUND_NORMAL) {
			PlayType = 0;
		}
		if (Type == SOUND_PATTERN::DX_SOUND_LOOP) {
			PlayType = DSBPLAY_LOOPING;
		}
		m_pDx_sound_buffer[Handle]->Play(0, 0, PlayType);
	}

	void DxSound::setVolume(int num, int Handle) {
		m_pDx_sound_buffer[Handle]->SetVolume(num);
	}

	void DxSound::stop(int Handle) {
		m_pDx_sound_buffer[Handle]->Stop();

	}

	void DxSound::initMem(int Handle) {
		m_pDx_sound_buffer[Handle]->Stop();
		m_pDx_sound_buffer[Handle]->Release();
		m_pDx_sound_buffer[Handle] = nullptr;
	}

	void DxSound::initMem() {
		for (int num = 0; num < m_pDx_sound_buffer.size(); num++) {
			initMem(num);
		}
		if (m_pDx_s8 != nullptr)
		{
			m_pDx_s8->Release();
			m_pDx_s8 = NULL;
		}
	}

	DxSound::DxSound(HWND hwnd) :
		m_Hwnd(hwnd) {
		auto result = DirectSoundCreate8(nullptr, &m_pDx_s8, nullptr);
		result = m_pDx_s8->SetCooperativeLevel(m_Hwnd, DSSCL_NORMAL);
	}

	DxSound::~DxSound() {
		initMem();
	}
}