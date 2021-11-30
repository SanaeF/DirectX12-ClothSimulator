#include "Application.h"
#include<thread>
#include "../../DirectXLib/Source/DxGraphics/DxGraphics.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "../../DirectXLib/Source/DxKeyConfig/DxKeyConfig.h"
#include "../../DirectXLib/Source/DxSound/DxSound.h"

bool Application::initialize() {
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	createGameWindow(m_Hwnd, m_Window_class);
	m_Dx12.reset(new lib::DirectX12Manager(m_Hwnd));
	m_Dx12->init(m_Pix);
	m_Sound.reset(new lib::DxSound(m_Hwnd));
	m_Key.reset(new lib::DxKeyConfig(m_Hwnd,m_Window_class));
	m_Graphics.reset(new lib::DxGraphics(m_Dx12));
	m_Key->keyInit(0);

	return true;
}


void Application::run() {
	int skirt;
	skirt = m_Graphics->load3D(L"./model/skirt/SchoolSkirt.pmx");//TestCloth,SchoolSkirt
	bool isSimulate = false;
	ClothForce cloth_f;
	cloth_f.is_self_collision = true;
	cloth_f.gravity = 9.8f;
	cloth_f.grid_mass = 1.f;
	cloth_f.damping = 0.3;
	cloth_f.dt = 2.6;
	cloth_f.windF(0, 0, 0);

	cloth_f.tensionParam(15,15);
	cloth_f.compressParam(15, 5);
	cloth_f.shareParam(15, 5);
	cloth_f.bendParam(25, 2);

	m_Graphics->setupClothSimulator(5, cloth_f, skirt);
	float angle = 0.0f;int spd = 1;int count = 0;float Move[3] = { 0,0,0 };MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {break;}
		m_Graphics->clearDraw();//画面の初期化
		m_Graphics->setArea(0, 0, 1920, 1440);//描画範囲の指定
		m_Graphics->draw3D(Move[0], Move[1], 6000 + Move[2], 1, angle);
		if(isSimulate)m_Graphics->clothSimProc(skirt);
		m_Graphics->screenFlip();//スワップチェイン

		m_Key->checkAll();//キー入力のセット
		if (m_Key->checkHitKey(DIK_C))spd = 20;//キーを押している間はCheckHitKeyの戻り値が増えていく
		else spd = 20;
		if (m_Key->checkHitKey(DIK_RIGHT) == 1)Move[0] += 2 * spd;
		if (m_Key->checkHitKey(DIK_LEFT) == 1)Move[0] -= 2 * spd;
		if (m_Key->checkHitKey(DIK_SPACE) == 1)Move[1] += 2 * spd;
		if (m_Key->checkHitKey(DIK_LSHIFT) == 1)Move[1] -= 2 * spd;
		if (m_Key->checkHitKey(DIK_UP) == 1)Move[2] += 2 * spd;
		if (m_Key->checkHitKey(DIK_DOWN) == 1)Move[2] -= 2 * spd;

		if (m_Key->checkHitKey(DIK_Z) == 1)angle += 0.002f * 10;
		if (m_Key->checkHitKey(DIK_X) == 1)angle -= 0.002f * 10;
		if (m_Key->checkHitKey(DIK_C) == 0)count++;
		if (m_Key->checkHitKey(DIK_R))m_Graphics->clothReset(skirt);
		isSimulate = false;
		if (m_Key->checkHitKey(DIK_P))isSimulate = true;
		if (count == 1920)count = 0;
		angle += 0.005f;
	}
}


void Application::setWindow(int width, int height, const char* window_name, const char* Name) {

	m_Win.cx = width;
	m_Win.cy = height;

	m_WinName[0] = window_name;
	m_WinName[1] = Name;
}

void Application::setGraphMode(int width, int height) {
	m_Pix.cx = width;
	m_Pix.cy = height;
}

SIZE Application::getWindowSize()const {
	SIZE ret;
	ret.cx = m_Win.cx;
	ret.cy = m_Win.cy;//_graphSize
	return ret;
}

SIZE Application::getGraphicSize()const {
	SIZE ret;
	ret.cx = m_Pix.cx;
	ret.cy = m_Pix.cy;
	return ret;
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {//ウィンドウが破棄されたら呼ばれます
		PostQuitMessage(0);//OSに対して「もうこのアプリは終わるんや」と伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
}

void Application::createGameWindow(HWND &hwnd, WNDCLASSEX &windowClass) {
	HINSTANCE hInst = GetModuleHandle(nullptr);
	//ウィンドウクラス生成＆登録
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
	windowClass.lpszClassName = _T(m_WinName[0]);//アプリケーションクラス名(適当でいいです)
	windowClass.hInstance = GetModuleHandle(0);//ハンドルの取得
	RegisterClassEx(&windowClass);//アプリケーションクラス(こういうの作るからよろしくってOSに予告する)

	RECT wrc = { 0,0, getWindowSize().cx, getWindowSize().cy };//ウィンドウサイズを決める
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//ウィンドウのサイズはちょっと面倒なので関数を使って補正する

	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	x = x - (wrc.right - wrc.left);
	y = y - (wrc.bottom - wrc.top);
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(windowClass.lpszClassName,//クラス名指定
		_T(m_WinName[1]),//タイトルバーの文字
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

void Application::showWin() {
	ShowWindow(m_Hwnd, SW_SHOW);//ウィンドウ表示
}

void Application::finaliz() {
	UnregisterClass(m_Window_class.lpszClassName, m_Window_class.hInstance);
}

Application& Application::getInstance() {
	static Application instance;
	return instance;
}

Application::Application(){
}

Application::~Application()
{
}