#include "Application.h"
#include<thread>
#include"Dx12Wrapper.h"
#include"PMDRenderer.h"
#include"DXPMDModel.h"

#include "DxGraph.h"
#include "DxKeyConfig.h"
#include "DxSound.h"

bool Application::Initialize() {
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow(_hwnd, _windowClass);
	mDxWr.reset(new Dx12Wrapper(_hwnd));
	mDxWr->Init(mPix);
	Sound.reset(new DxSound(_hwnd));
	Key.reset(new DxKeyConfig(_hwnd,_windowClass));
	Graph.reset(new DxGraph(mDxWr));
	Key->KeyInit(0);
	return true;
}


void Application::Run() {
	int imageHandle[2];
	int HandleLif;
	int MusicHandle;
	imageHandle[0] = Graph->Load2D(L"./dat/back.png");
	imageHandle[1] = Graph->Load2D(L"./dat/ochiful.png");
	HandleLif = Graph->Load2D(L"./dat/Bullet02.png");
	int text_img = Graph->Load2D(L"./dat/shadow_wing.png");
	MusicHandle = Sound->LoadFile("./dat/music.wav");
	Sound->SetVolume(-3000, MusicHandle);
	Sound->Play(MusicHandle, SOUND::eDXSOUND_LOOP);
	float angle = 0.0f;
	int spd = 1;
	int count = 0;
	float Move[2] = { 0,0 };
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			break;
		}
		Key->CheckAll();
		if (Key->CheckHitKey(DIK_C) == 1)spd = 20;
		else if (Key->CheckHitKey(DIK_LSHIFT) == 1)spd = 1;
		else spd = 10;
		if (Key->CheckHitKey(DIK_UP) == 1)Move[0] += 2 * spd;
		if (Key->CheckHitKey(DIK_DOWN) == 1)Move[0] -= 2 * spd;
		if (Key->CheckHitKey(DIK_RIGHT) == 1)Move[1] += 2 * spd;
		if (Key->CheckHitKey(DIK_LEFT) == 1)Move[1] -= 2 * spd;
		if (Key->CheckHitKey(DIK_Z) == 1)angle += 0.002f * 10;
		if (Key->CheckHitKey(DIK_X) == 1)angle -= 0.002f * 10;
		if (Key->CheckHitKey(DIK_C) == 0)count++;
		if (count == 1920)count = 0;
		Graph->ClearDraw();
		//if (count < 960)Graph->SetArea(0, 0, (count % 960) * 2, 1440);
		//if (960 < count)Graph->SetArea((count % 960) * 2, 0, 1920, 1440);
		Graph->DrawPrototype2D(0, 0, 1, 0, imageHandle[1]);
		Graph->SetArea(0, 0, 1920, 1440);
		for (int i = 0; i < 5; i++)Graph->DrawPrototype2D(-Move[1] - i * 100, -Move[0], 1, angle, HandleLif);
		for (int i = 0; i < 1000; i++)Graph->DrawPrototype2D(Move[1] + i * 100, Move[0]+100*(i%2), 1, -angle, HandleLif);
		if(count>960)Graph->DrawPrototype2D(0, 0, 2, -angle, HandleLif);
		//if (count < 960)Graph->SetArea((count % 960) * 2, 0, 1920, 1440);
		//if (960 < count)Graph->SetArea(0, 0, (count % 960) * 2, 1440);
		//Graph->DrawPrototype2D(Move[1], Move[0], 1, angle, imageHandle[0]);

		Graph->ScreenFlip();
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