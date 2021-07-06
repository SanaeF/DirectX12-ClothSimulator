#include "DxKeyConfig.h"
#include <cassert>
namespace lib {
    void DxKeyConfig::setInterface(LPDIRECTINPUTDEVICE8 Input) {
        auto result = m_Lp_DI->CreateDevice(GUID_SysKeyboard, &Input, NULL);
        if (FAILED(result)) {
            m_Lp_DI->Release();
            assert(0 && "CreateDevice ERROR");
        }
        m_Lp_keyboard = Input;
    }

    void DxKeyConfig::setInputData(LPDIRECTINPUTDEVICE8 Input) {
        auto result = Input->SetDataFormat(&c_dfDIKeyboard);
        if (FAILED(result)) {
            Input->Release();
            m_Lp_DI->Release();
            assert(0 && "SetDataFormat ERROR");
        }
        m_Lp_keyboard = Input;
    }

    void DxKeyConfig::keyCooperative(HWND hwnd, LPDIRECTINPUTDEVICE8 Input) {
        auto result = Input->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
        if (FAILED(result)) {
            Input->Release();
            m_Lp_DI->Release();
            assert(0 && "SetCooperativeLevel ERROR");
        }
    }

    void DxKeyConfig::keyInit(LPDIRECTINPUTDEVICE8 Input) {
        setInterface(m_Lp_keyboard);

        setInputData(m_Lp_keyboard);
        keyCooperative(m_Hwnd, m_Lp_keyboard);

        m_Lp_keyboard->Acquire();
    }

    int DxKeyConfig::checkAll(int CheckType) {

        if (CheckType == DX_INPUT_ALL) {
            auto result = m_Lp_keyboard->GetDeviceState(sizeof(m_Key), m_Key);
            if (FAILED(result)) {
                // Ž¸”s‚È‚çÄŠJ‚³‚¹‚Ä‚à‚¤ˆê“xŽæ“¾
                m_Lp_keyboard->Acquire();
                m_Lp_keyboard->GetDeviceState(sizeof(m_Key), m_Key);
                return -1;
            }
        }

        return 0;
    }

    int DxKeyConfig::checkHitKey(int KeyCode) {
        if (m_Key[KeyCode] & 0x80)return 1;
        return 0;
    }


    DxKeyConfig::DxKeyConfig(HWND hwnd, WNDCLASSEX& windowClass) :m_Hwnd(hwnd)
    {
        ZeroMemory(m_Key, sizeof(m_Key));

        auto result = DirectInput8Create(windowClass.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_Lp_DI, NULL);
        if (FAILED(result)) {
            assert(0 && "DirectInput8‚Ìì¬‚ÉŽ¸”s\n");
        }
    }

    DxKeyConfig::~DxKeyConfig() {
        m_Lp_keyboard->Release();
        m_Lp_DI->Release();
    }
}