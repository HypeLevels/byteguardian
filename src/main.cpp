#include "../include.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT nCmdShow) {
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