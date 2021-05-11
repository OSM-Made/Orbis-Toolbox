#include "Common.h"
#include "Settings_Menu.h"

extern "C"
{
	int module_start()
	{
		klog("!! Hello World !!\n");

		Mono::Init();

		UI::Utilities::SetVersionString("5.05 OSM's Cool Firmware");
		Settings_Menu::Init();
		//Title_Menu::Init();

		Notify("Orbis Toolbox: Loaded!");

		return 0;
	}

	int module_stop()
	{
		klog("!! BYE !!\n");

		Notify("Orbis Toolbox: Unloading...");

		Settings_Menu::Term();
		//Title_Menu::Term();

		return 0;
	}

	void _start()
	{

	}
}