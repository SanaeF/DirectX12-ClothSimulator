#include "DxWavData.h"
#include <dsound.h>
#include <stdio.h>


bool DxWavData::mWavProp() {
	riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// dataチャンクに進入する
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,	// MMIOハンドル
		&riffck_info,	// 進入したチャンクの情報
		nullptr,	// 親チャンク
		MMIO_FINDRIFF))// 情報の種類
	{
		// 進入失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}
	return true;
}

bool DxWavData::mFmtProp() {
	ck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(mmio_handle, &ck_info, &riffck_info, MMIO_FINDCHUNK))
	{
		// fmtチャンクがない
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	read_size = mmioRead(
		mmio_handle,			// ハンドル
		(HPSTR)&mWaveData.WavFormat,	// 読み込み用バッファ
		sizeof(mWaveData.WavFormat));	// バッファサイズ

	if (read_size != sizeof(mWaveData.WavFormat))
	{
		// 読み込みサイズが一致してないのでエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットチェック
	if (mWaveData.WavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		// フォーマットエラー
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	if (mmioAscend(mmio_handle, &ck_info, 0) != MMSYSERR_NOERROR)
	{
		mmioClose(mmio_handle, 0);
		return false;
	}
	return true;
}

bool DxWavData::mDataProp() {
	ck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(
			mmio_handle,
			&ck_info, 
			&riffck_info,
			MMIO_FINDCHUNK
		) != MMSYSERR_NOERROR)
	{
		// 進入失敗
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}
	return true;
}

bool DxWavData::mLoadSound() {
	mWaveData.Size = ck_info.cksize;
	char* sound_buffer = new char[ck_info.cksize];
	read_size = mmioRead(mmio_handle, sound_buffer, ck_info.cksize);
	if (read_size != ck_info.cksize)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		delete[] sound_buffer;
		return false;
	}
	mWaveData.SoundBuffer = sound_buffer;
	return true;
}

int DxWavData::LoadFile(const char* file_name) {

	mmio_handle = mmioOpen(
		(char*)file_name,	// ファイル名
		nullptr,			// MMIO情報
		MMIO_READ);		// モード
	if (mmio_handle == nullptr)return -1;

	if (!mWavProp())return -1;
	if (!mFmtProp())return -1;
	if (!mDataProp())return -1;
	if (!mLoadSound())return -1;

	mmioClose(mmio_handle, MMIO_FHOPEN);

	return 0;
}
