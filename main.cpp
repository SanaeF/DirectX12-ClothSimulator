#include"Application.h"
#include<Windows.h>
#ifdef _DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	auto& app = Application::Instance();
	app.SetWindow(1280, 960, "DirectX", "DirectX12");
	app.SetGraphMode(1920, 1440);
	if (!app.Initialize()) {
		return -1;
	}
	app.ShowWin();

	app.Run();

	app.Finaliz();
	return 0;
}
