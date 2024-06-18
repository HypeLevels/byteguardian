#include "../include.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT nCmdShow) {
	std::rename(path().c_str(), std::string(random_string(10) + ".exe").c_str());
	config::refreshConfigs();
	gui::setupWindow(hInstance);
	gui::setupDX11(nCmdShow);
	gui::setupImgui();

	std::thread(misc::bunnyhop).detach();
	std::thread(aim::triggerbot).detach();

	while (process.ProcessIsOpen("cs2.exe") && gui::shouldRun) {
		gui::beginRender();
		gui::render();
		gui::endRender();
	}

	gui::cleanUp();
}