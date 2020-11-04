#include "Application.h"
#include"Dx12Wrapper.h"
#include"PMDRenderer.h"
#include"DXPMDModel.h"

#include "DXGraph.h"

#include "DxSound.h"

bool Application::Initialize() {
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow(_hwnd, _windowClass);
	mDxWr.reset(new Dx12Wrapper(_hwnd));
	mDxWr->Init(mPix);
	Sound.reset(new DxSound(_hwnd));
	Graph.reset(new DXGraph(mDxWr));
	return true;
}


void Application::Run() {//خليط عضوي
	int imageHandle[2];
	int MusicHandle;

	imageHandle[0] = Graph->Load2D(L"./dat/backB.png");
	imageHandle[1] = Graph->Load2D(L"./dat/ochiful.png");
	int text_img = Graph->Load2D(L"./dat/shadow_wing.png");


	MusicHandle = Sound->LoadFile("./dat/music.wav");
	Sound->SetVolume(-2700, MusicHandle);
	Sound->Play(MusicHandle, SOUND::eDXSOUND_LOOP);


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
		mDxWr->ClearScreen();
		Graph->SetDrawArea(0, 0, 1920, 1440);
		angle += 0.001f;
		Graph->DrawPrototype2D(0, imageHandle[1]);
		Graph->DrawPrototype2D(angle, imageHandle[0]);
		Graph->DrawPrototype2D(0, text_img);
		
		
		mDxWr->ScreenFlip();

	}
}




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

	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	x = x - (wrc.right - wrc.left);
	y = y - (wrc.bottom - wrc.top);
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(windowClass.lpszClassName,//クラス名指定
		_T(winName[1]),//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウです
		x/2,//表示X座標はOSにお任せします
		y/2,//表示Y座標はOSにお任せします
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

void Application::RunTest() {
	int imageHandle;
	mDxWr.reset(new Dx12Wrapper(_hwnd));
	mPMDRenderer.reset(new PMDRenderer(mDxWr)); 

	mPMDRenderer->Init();
	mPMDModel.reset(new DXPMDModel(mDxWr));

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

		mDxWr->ClearScreen();

		mDxWr->Update();

		mPMDRenderer->Update();

		mPMDRenderer->BeforeDraw();

		mDxWr->Draw(mPMDRenderer);

		mDxWr->SetScene();

		mPMDModel->Update();
		mPMDModel->Draw(imageHandle);

		mDxWr->CommandClear();

		mDxWr->ScreenFlip();

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