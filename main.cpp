#include"Application.h"
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto& app = Application::Instance();

	app.SetWindow(1280,960,"DirectX","DirectX12");
	app.SetGraphMode(1920*1.5,1440 * 1.5);
	if (!app.Initialize()) {
		return -1;
	}
	app.ShowWin();

	app.Run2();

	app.Finaliz();
	return 0;
}