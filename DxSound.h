#pragma once
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

#include<Windows.h>
#include <dsound.h>
#include <vector>
#include<map>
enum SOUND {
	eDXSOUND_NORMAL,
	eDXSOUND_LOOP,
};

class DxWavData;

class DxSound {
private:

	HWND mHwnd;

	IDirectSound8* pDxS8;

	DSBUFFERDESC mDxS_BuffDesc;

	std::map<int , LPDIRECTSOUNDBUFFER>pDxSoundBuffer;


	static int mHandleCount;

public:

	DxSound(HWND hwnd);
	~DxSound();

	//サウンドファイルを読み込む
	int LoadFile(const char* file_name);

	void SetVolume(int num, int Handle);

	//ハンドルのサウンドを再生する
	void Play(int Handle, int Type);

	//ハンドルのサウンドを停止する
	void Stop(int Handle);

	//ハンドルデータのサウンドをメモリから消す
	void InitMem(int Handle);

	//サウンドを全部メモリから消す
	void InitMem();
};