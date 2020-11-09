#include "DirectXLib.h"
#include<tchar.h>

#include"Dx12Wrapper.h"
#include"PMDRenderer.h"
#include"DXPMDModel.h"
#include "DxGraph.h"
#include "DxKeyConfig.h"
#include "DxSound.h"
//
//LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
//	if (msg == WM_DESTROY) {//ウィンドウが破棄されたら呼ばれます
//		PostQuitMessage(0);//OSに対して「もうこのアプリは終わるんや」と伝える
//		return 0;
//	}
//	return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
//}
//
//
//bool DirectXLib::Initialize() {
//	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
//	CreateGameWindow(_hwnd, _windowClass);
//	mDxWr.reset(new Dx12Wrapper(_hwnd));
//	mDxWr->Init(mPix);
//	Sound.reset(new DxSound(_hwnd));
//	Graph.reset(new DxGraph(mDxWr));
//	KeyConfig.reset(new DxKeyConfig(_windowClass));
//	return true;
//}
//
//void DirectXLib::Finaliz() {
//	UnregisterClass(_windowClass.lpszClassName, _windowClass.hInstance);
//}
//
//void DirectXLib::CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass) {
//	HINSTANCE hInst = GetModuleHandle(nullptr);
//	//ウィンドウクラス生成＆登録
//	windowClass.cbSize = sizeof(WNDCLASSEX);
//	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
//	windowClass.lpszClassName = _T(winName[0]);//アプリケーションクラス名(適当でいいです)
//	windowClass.hInstance = GetModuleHandle(0);//ハンドルの取得
//	auto result = RegisterClassEx(&windowClass);//アプリケーションクラス(こういうの作るからよろしくってOSに予告する)
//
//	RECT wrc = { 0,0, GetWindowSize().cx, GetWindowSize().cy };//ウィンドウサイズを決める
//	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//ウィンドウのサイズはちょっと面倒なので関数を使って補正する
//
//	int x = GetSystemMetrics(SM_CXSCREEN);
//	int y = GetSystemMetrics(SM_CYSCREEN);
//	x = x - (wrc.right - wrc.left);
//	y = y - (wrc.bottom - wrc.top);
//	//ウィンドウオブジェクトの生成
//	hwnd = CreateWindow(
//		windowClass.lpszClassName,//クラス名指定
//		_T(winName[1]),//タイトルバーの文字
//		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウです
//		x / 2,//表示X座標はOSにお任せします
//		y / 2,//表示Y座標はOSにお任せします
//		wrc.right - wrc.left,//ウィンドウ幅
//		wrc.bottom - wrc.top,//ウィンドウ高
//		nullptr,//親ウィンドウハンドル
//		nullptr,//メニューハンドル
//		windowClass.hInstance,//呼び出しアプリケーションハンドル
//		nullptr);//追加パラメータ
//	if (hwnd == NULL)assert(0);
//	return;
//}
//
//void DirectXLib::SetShowWindow() {
//	ShowWindow(_hwnd, SW_SHOW);//ウィンドウ表示
//}
//
//
//void DirectXLib::SetWindow(int width, int height, const char* window_name, const char* Name) {
//	mWin.cx = width;
//	mWin.cy = height;
//	winName[0] = window_name;
//	winName[1] = Name;
//}
//
//void DirectXLib::SetGraphMode(int width, int height) {
//	mPix.cx = width;
//	mPix.cy = height;
//}
//
//SIZE DirectXLib::GetWindowSize()const {
//	SIZE ret;
//	ret.cx = mWin.cx;
//	ret.cy = mWin.cy;//_graphSize
//	return ret;
//}
//
//SIZE DirectXLib::GetGraphicSize()const {
//	SIZE ret;
//	ret.cx = mPix.cx;
//	ret.cy = mPix.cy;
//	return ret;
//}
//
//DirectXLib::DirectXLib(){
//	this->SetWindow(100, 100, "TestWindow", "DirectXLibrary");
//	this->SetGraphMode(100, 100);
//}
//DirectXLib::~DirectXLib() {
//
//}