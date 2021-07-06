#pragma once
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

#include<Windows.h>
#include <dsound.h>
#include <vector>
#include<map>
namespace lib {
	enum SOUND_PATTERN {
		DX_SOUND_NORMAL,//普通に再生します('ω')
		DX_SOUND_LOOP,//音楽をループ再生します(｀・ω・´)
	};
	class InputWavData;
	class DxSound {
	private:
		HWND m_Hwnd;
		IDirectSound8* m_pDx_s8;
		DSBUFFERDESC m_Dxs_buff_desc;
		std::map<int, LPDIRECTSOUNDBUFFER>m_pDx_sound_buffer;

		static int m_Handle_count;
	public:
		DxSound(HWND hwnd);
		~DxSound();
		//サウンドファイルを読み込みます。
		int loadFile(const char* file_name);
		//サウンドの怨霊を調節します('Д')
		void setVolume(int num, int Handle);
		//ハンドルのサウンドを再生してやるよ
		void play(int Handle, int Type);
		//ハンドルのサウンドを停止します。
		void stop(int Handle);
		//ハンドルデータのサウンドをメモリから削除！
		void initMem(int Handle);
		//サウンドを全部メモリから削除！
		void initMem();
	};
}