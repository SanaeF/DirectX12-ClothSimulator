#include "Application.h"
#include"Dx12Wrapper.h"
#include "DX_Resource.h"
#include"PMDRenderer.h"
#include"PMDActor.h"
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


bool Application::Init() {
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow(_hwnd, _windowClass);
	//DirectX12ラッパー生成＆初期化
	return true;
}

void Application::Run() {
	pDx12.reset(new Dx12Wrapper(_hwnd));
	//pPMDRenderer[1].reset(new PMDRenderer(*pDx12)); 
	//pPMDRenderer[0].reset(new PMDRenderer(*pDx12));
	//pPMDActor.reset(new PMDActor("model/白河ことり（本校制服）ミク.pmd", *pPMDRenderer[0]));
	//pPMDActor.reset(new PMDActor("model/朝倉音夢（本校制服）ミク.pmd", *pPMDRenderer[1]));//テクスチャを上書きしている
	//pPMDActor->LoadVMDFile("motion/motion.vmd", "pose");
	//pPMDActor->PlayAnimation();
	float angle = 0.0f;
	MSG msg = {};
	unsigned int frame = 0;
	//while (true) {
	//	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//	//もうアプリケーションが終わるって時にmessageがWM_QUITになる
	//	if (msg.message == WM_QUIT) {
	//		break;
	//	}
	//	//全体の描画準備
	//	pDx12->BeginDraw();
	//	//PMD用の描画パイプラインに合わせる
	//	pDx12->CommandList()->SetPipelineState(pPMDRenderer[1]->GetPipelineState());
	//	pDx12->CommandList()->SetPipelineState(pPMDRenderer[0]->GetPipelineState());
	//	//ルートシグネチャもPMD用に合わせる
	//	pDx12->CommandList()->SetGraphicsRootSignature(pPMDRenderer[1]->GetRootSignature());
	//	pDx12->CommandList()->SetGraphicsRootSignature(pPMDRenderer[0]->GetRootSignature());
	//	pDx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	pDx12->SetScene();

	//	pPMDActor->Update();
	//	pPMDActor->Draw();

	//	pDx12->EndDraw();

	//	//フリップ
	//	pDx12->Swapchain()->Present(1, 0);
	//}
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