#pragma once
//#include<Windows.h>
//#include<memory>
//
//class Dx12Wrapper;
//class PMDRenderer;
//class DXPMDModel;
//
//class DxGraph;
//class DxKeyConfig;
//class DxSound;
//
/////シングルトンクラス
//class DirectXLib {
//private:
//	SIZE mWin, mPix;
//
//	const char* winName[2];
//
//	//ここに必要な変数(バッファやヒープなど)を書く
//	//ウィンドウ周り
//	WNDCLASSEX _windowClass;
//	HWND _hwnd;
//	std::shared_ptr<Dx12Wrapper> mDxWr;
//	std::shared_ptr<PMDRenderer> mPMDRenderer;
//	std::shared_ptr<DXPMDModel> mPMDModel;
//	std::shared_ptr<DxGraph>Graph;
//	std::shared_ptr<DxKeyConfig>Key;
//	std::shared_ptr<DxSound>Sound;
//	//ゲーム用ウィンドウの生成
//	void CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass);
//
//
//
//	DirectXLib();
//	DirectXLib(const DirectXLib&) = delete;
//	void operator=(const DirectXLib&) = delete;
//
//public:
//
//	bool Initialize();
//
//	void ShowWin();
//
//	void Finaliz();
//
//	void SetWindow(int width, int height, const char* window_name, const char* name);
//
//	void SetGraphMode(int width, int height);
//
//	SIZE GetWindowSize()const;
//
//	SIZE GetGraphicSize()const;
//
//	static DirectXLib& Instance();
//	~DirectXLib();
//
//};
