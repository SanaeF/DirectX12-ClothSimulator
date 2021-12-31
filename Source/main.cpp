#include"Application/Application.h"
#include<Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto& app = Application::getInstance();
	app.setWindow(1280, 960, "布アニメーション", "PlasticSkirt");
	app.setGraphMode(1920, 1440);
	if (!app.initialize()) {
		return -1;
	}
	app.showWin();

	app.run();

	app.finaliz();
	return 0;
}