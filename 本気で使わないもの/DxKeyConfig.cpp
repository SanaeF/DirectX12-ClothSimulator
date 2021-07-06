#include "DxKeyConfig.h"
#include <cassert>

void DxKeyConfig::mSetInterface(LPDIRECTINPUTDEVICE8 Input) {
    auto result = lpDI->CreateDevice(GUID_SysKeyboard, &Input, NULL);
    if (FAILED(result)) {
        lpDI->Release();
        assert(0 && "CreateDevice ERROR");
    }
    lpKeyboard = Input;
}

void DxKeyConfig::mSetInputData(LPDIRECTINPUTDEVICE8 Input) {
    auto result = Input->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(result)) {
        Input->Release();
        lpDI->Release();
        assert(0 && "SetDataFormat ERROR");
    }
    lpKeyboard = Input;
}

void DxKeyConfig::mKeyCooperative(HWND hwnd, LPDIRECTINPUTDEVICE8 Input) {
    auto result = Input->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    if (FAILED(result)) {
        Input->Release();
        lpDI->Release();
        assert(0 && "SetCooperativeLevel ERROR");
    }
}

void DxKeyConfig::KeyInit(LPDIRECTINPUTDEVICE8 Input) {
    mSetInterface(lpKeyboard);

    mSetInputData(lpKeyboard);
    mKeyCooperative(mHwnd, lpKeyboard);

    lpKeyboard->Acquire();
}

int DxKeyConfig::CheckAll(int CheckType) {

    if (CheckType == eINPUT_ALL) {
        auto result = lpKeyboard->GetDeviceState(sizeof(mKey), mKey);
        if (FAILED(result)) {
            // Ž¸”s‚È‚çÄŠJ‚³‚¹‚Ä‚à‚¤ˆê“xŽæ“¾
            lpKeyboard->Acquire();
            lpKeyboard->GetDeviceState(sizeof(mKey), mKey);
            return -1;
        }
    }

    return 0;
}

int DxKeyConfig::CheckHitKey(int KeyCode) {
    if (mKey[KeyCode] & 0x80)return 1;
    return 0;
}


DxKeyConfig::DxKeyConfig(HWND hwnd, WNDCLASSEX& windowClass):mHwnd(hwnd)
{
    ZeroMemory(mKey, sizeof(mKey));

    auto result = DirectInput8Create(windowClass.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
    if (FAILED(result)) {
        assert(0 && "DirectInput8‚Ìì¬‚ÉŽ¸”s\n");
    }
}

DxKeyConfig::~DxKeyConfig() {
    lpKeyboard->Release();
    lpDI->Release();
}

