#include "Common.h"
#include "Settings_Menu.h"

int* __errno_location();

extern "C"
{


	int module_start()
	{
		klog("!! Hello World !!\n");

		mount_large_fs("/dev/da0x4.crypt", "/system", "exfatfs", "511", MNT_UPDATE);

		Mono::Init();

		//UISystem.ScreenWidth

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