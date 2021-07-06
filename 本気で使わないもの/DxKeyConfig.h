#pragma once
#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

enum DxKeyType {
	eINPUT_KEY,
	eINPUT_PAD,
	eINPUT_MOUSE,
	eINPUT_ALL
};

class DxKeyConfig {
private:

	LPDIRECTINPUT8 lpDI = nullptr;

	LPDIRECTINPUTDEVICE8 lpKeyboard = nullptr;

	BYTE mKey[256];

	HWND mHwnd;

public:

	DxKeyConfig(HWND hwnd, WNDCLASSEX& windowClass);
	~DxKeyConfig();

	void KeyInit(LPDIRECTINPUTDEVICE8 Input);

	int CheckAll(int CheckType = eINPUT_ALL);

	int CheckHitKey(int KeyCode);

private:

	void mSetInterface(LPDIRECTINPUTDEVICE8 Input);

	void mSetInputData(LPDIRECTINPUTDEVICE8 Input);

	void mKeyCooperative(HWND hwnd, LPDIRECTINPUTDEVICE8 Input);

};