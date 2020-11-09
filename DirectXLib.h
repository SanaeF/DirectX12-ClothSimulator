#pragma once
#include<Windows.h>
#include<memory>
class Dx12Wrapper;
class PMDRenderer;
class DXPMDModel;
class DxGraph;
class DxKeyConfig;
class DxSound;

class DirectXLib {
private:

	SIZE mWin, mPix;

	const char* winName[2];

	WNDCLASSEX _windowClass;
	HWND _hwnd;

	std::shared_ptr<Dx12Wrapper> mDxWr;
	std::shared_ptr<PMDRenderer> mPMDRenderer;
	std::shared_ptr<DXPMDModel> mPMDModel;
	std::shared_ptr<DxGraph>Graph;
	std::shared_ptr<DxSound>Sound;
	std::shared_ptr<DxKeyConfig>KeyConfig;

public:
	///èâä˙âª
	bool Initialize();

	///ÉãÅ[ÉvãNìÆ
	void SetShowWindow();

	///å„èàóù
	void Finaliz();


	void SetWindow(int width, int height, const char* window_name, const char* name);

	void SetGraphMode(int width, int height);

	DirectXLib();
	~DirectXLib();

private:

	void CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass);

	SIZE GetWindowSize()const;

	SIZE GetGraphicSize()const;

};