#include "Common.h"
#include "Settings_Menu.h"
#include "CPU_Monitor.h"

extern "C"
{
	int module_start()
	{
		//sceKernelSetProcessName
		klog("!! Hello World !!\n");

		
		int cpuUsed, cpuTotal, gpuUsed, gpuTotal;
		Get_Page_Table_Stats(&cpuUsed, &cpuTotal, &gpuUsed, &gpuTotal);

		klog("RAM: %u MB / %u MB\nVRAM: %u MB / %u MB\n", cpuUsed, cpuTotal, gpuUsed, gpuTotal);

		Mono::Init();

		//UI::Utilities::SetVersionString("5.05 OSM's Cool Firmware");
		CPU_Monitor::Init();
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
		CPU_Monitor::Term();
		//Title_Menu::Term();

		return 0;
	}

	void _start()
	{

	}
}