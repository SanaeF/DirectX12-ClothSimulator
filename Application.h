#pragma once
#include<Windows.h>
#include<tchar.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
#include<map>
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include<memory>

class Dx12Wrapper;
class PMDRenderer;
class PMDActor;
///シングルトンクラス
class Application{
private:
	SIZE mWin, mPix;

	const char* winName[2];

	//ここに必要な変数(バッファやヒープなど)を書く
	//ウィンドウ周り
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	std::shared_ptr<Dx12Wrapper> pDx12;
	std::shared_ptr<PMDRenderer> pPMDRenderer;
	std::shared_ptr<PMDActor> pPMDActor;

	//ゲーム用ウィンドウの生成
	void CreateGameWindow(HWND &hwnd, WNDCLASSEX &windowClass);



	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

public:

	///初期化
	bool Init();

	///ループ起動
	void ShowWin();

	void Run();

	///後処理
	void Finaliz();



	void SetWindow(int width, int height, const char* window_name, const char* name);

	void SetGraphMode(int width, int height);

	SIZE GetWindowSize()const;

	SIZE GetGraphicSize()const;


	static Application& Instance();
	~Application();
};
