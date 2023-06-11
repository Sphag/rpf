#include <pch.h>

#include <app/app.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	rpf::app::APPLICATION::INIT_SETTINGS appSettings;
	appSettings.hInstance = hInstance;
	appSettings.nCmdShow = nCmdShow;
	appSettings.windowClassName = L"Engine";
	appSettings.windowName = L"Engine";
	appSettings.wndWidth = 1920;
	appSettings.wndHeight = 1080;

	rpf::app::APPLICATION app(appSettings);

	app.Init();
	app.Run();
	app.Term();

	return 0;
}