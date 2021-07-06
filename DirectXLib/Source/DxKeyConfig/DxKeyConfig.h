#pragma once
#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
namespace lib {
	enum DxKeyType {
		DX_INPUT_KEY,
		DX_INPUT_PAD,
		DX_INPUT_MOUSE,
		DX_INPUT_ALL
	};
	class DxKeyConfig {
	private:
		LPDIRECTINPUT8 m_Lp_DI = nullptr;
		LPDIRECTINPUTDEVICE8 m_Lp_keyboard = nullptr;
		BYTE m_Key[256];
		HWND m_Hwnd;
	public:
		DxKeyConfig(HWND hwnd, WNDCLASSEX& windowClass);
		~DxKeyConfig();
		void keyInit(LPDIRECTINPUTDEVICE8 Input);
		int checkAll(int CheckType = DX_INPUT_ALL);
		int checkHitKey(int KeyCode);
	private:
		void setInterface(LPDIRECTINPUTDEVICE8 Input);
		void setInputData(LPDIRECTINPUTDEVICE8 Input);
		void keyCooperative(HWND hwnd, LPDIRECTINPUTDEVICE8 Input);
	};
}