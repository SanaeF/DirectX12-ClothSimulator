#pragma once
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

#include<Windows.h>
#include <dsound.h>
#include <vector>
#include<map>
enum SOUND {
	eDXSOUND_NORMAL,//普通に再生します('ω')
	eDXSOUND_LOOP,//音楽をループ再生します(｀・ω・´)
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

	//サウンドファイルを読み込みます。
	int LoadFile(const char* file_name);

	//サウンドの怨霊を調節します('Д')
	void SetVolume(int num, int Handle);

	//ハンドルのサウンドを再生してやるよ
	void Play(int Handle, int Type);

	//ハンドルのサウンドを停止します。
	void Stop(int Handle);

	//ハンドルデータのサウンドをメモリから削除！
	void InitMem(int Handle);

	//サウンドを全部メモリから削除！
	void InitMem();
};