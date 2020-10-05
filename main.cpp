#include"Application.h"
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto& app = Application::Instance();

	app.SetWindow(1280,960,"DirectX","ƒeƒXƒg");
	app.SetGraphMode(1920,1440);
	if (!app.Init()) {
		return -1;
	}
	app.ShowWin();

	app.Run();

	app.Finaliz();
	return 0;
}