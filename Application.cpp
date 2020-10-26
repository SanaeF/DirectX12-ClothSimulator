#include "Application.h"
#include"Dx12Wrapper.h"
#include"PMDRenderer.h"
#include"DXPMDModel.h"

#include "DXGraph.h"

void Application::SetWindow(int width, int height, const char* window_name, const char* Name) {

	mWin.cx = width;
	mWin.cy = height;

	winName[0] = window_name;
	winName[1] = Name;
}
void Application::SetGraphMode(int width, int height) {
	mPix.cx = width;
	mPix.cy = height;
}

SIZE Application::GetWindowSize()const {
	SIZE ret;
	ret.cx = mWin.cx;
	ret.cy = mWin.cy;//_graphSize
	return ret;
}
SIZE Application::GetGraphicSize()const {
	SIZE ret;
	ret.cx = mPix.cx;
	ret.cy = mPix.cy;
	return ret;
}
//面倒だけど書かなあかんやつ
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {//ウィンドウが破棄されたら呼ばれます
		PostQuitMessage(0);//OSに対して「もうこのアプリは終わるんや」と伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
}
void Application::CreateGameWindow(HWND &hwnd, WNDCLASSEX &windowClass) {
	HINSTANCE hInst = GetModuleHandle(nullptr);
	//ウィンドウクラス生成＆登録
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
	windowClass.lpszClassName = _T(winName[0]);//アプリケーションクラス名(適当でいいです)
	windowClass.hInstance = GetModuleHandle(0);//ハンドルの取得
	RegisterClassEx(&windowClass);//アプリケーションクラス(こういうの作るからよろしくってOSに予告する)

	RECT wrc = { 0,0, GetWindowSize().cx, GetWindowSize().cy };//ウィンドウサイズを決める
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//ウィンドウのサイズはちょっと面倒なので関数を使って補正する
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(windowClass.lpszClassName,//クラス名指定
		_T(winName[1]),//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウです
		CW_USEDEFAULT,//表示X座標はOSにお任せします
		CW_USEDEFAULT,//表示Y座標はOSにお任せします
		wrc.right - wrc.left,//ウィンドウ幅
		wrc.bottom - wrc.top,//ウィンドウ高
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		windowClass.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//追加パラメータ

}

void Application::ShowWin() {
	ShowWindow(_hwnd, SW_SHOW);//ウィンドウ表示
}

bool Application::Initialize() {
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow(_hwnd, _windowClass);
	//DirectX12ラッパー生成＆初期化
	return true;
}


void Application::Run2() {
	int imageHandle[2];
	mDirectX_R.reset(new Dx12Wrapper(_hwnd));
	Graph.reset(new DXGraph(mDirectX_R));
	
	imageHandle[0] = Graph->Load2D(L"./dat/backB.png");
	imageHandle[1] = Graph->Load2D(L"./dat/ochiful.png");
	int text_img = Graph->Load2D(L"./dat/txt.png");
	float angle = 0.0f;
	MSG msg = {};
	unsigned int frame = 0;
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			break;
		}
		angle += 0.002f;
		Graph->DrawPrototype2D(0, imageHandle[1]);
		Graph->DrawPrototype2D(angle, imageHandle[0]);
		Graph->DrawPrototype2D(0, text_img);
		mDirectX_R->ClearDraw();

		mDirectX_R->ScreenFlip();

	}
}


void Application::Run() {
	int imageHandle;
	mDirectX_R.reset(new Dx12Wrapper(_hwnd));
	mPMDRenderer.reset(new PMDRenderer(mDirectX_R)); 

	mPMDRenderer->Init();
	mPMDModel.reset(new DXPMDModel(mDirectX_R));

	imageHandle = mPMDModel->LoadPMDFile("model/鷺澤美咲ミク.pmd");
	mPMDModel->LoadVMDFile("motion/motion.vmd", "pose");

	mPMDModel->PlayAnimation();
	//mPMDRenderer->AddActor(mPMDModel);

	float angle = 0.0f;
	MSG msg = {};
	unsigned int frame = 0;
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (msg.message == WM_QUIT) {
			break;
		}

		mDirectX_R->BeginDraw();

		mDirectX_R->Update();

		mPMDRenderer->Update();

		mPMDRenderer->BeforeDraw();

		mDirectX_R->Draw(mPMDRenderer);

		mDirectX_R->SetScene();

		mPMDModel->Update();
		mPMDModel->Draw(imageHandle);

		mDirectX_R->ClearDraw();

		mDirectX_R->ScreenFlip();

	}
}

void Application::Finaliz() {
	UnregisterClass(_windowClass.lpszClassName, _windowClass.hInstance);
}

Application& Application::Instance() {
	static Application instance;
	return instance;
}

Application::Application(){
}

Application::~Application()
{
}