#include "Common.h"
#include "Settings_Menu.h"
#include "System_Monitor.h"
#include "GamePad.h"

extern "C"
{
	int module_start()
	{
		klog("!! Hello World !!\n");

		Mono::Init();

		if (GamePad::IsDown(GamePad::Buttons::Left | GamePad::Buttons::Triangle))
		{
			Notify("Orbis Toolbox: Aborting Launch!!");
			return 0;
		}

		//Sce.PlayStation.Core.Runtime DiagnosticsNative GetGraphicsMemoryStatistics 
		//TODO: Get Address and offset take a look in IDA see if it calls imports.

		System_Monitor::Init();
		Settings_Menu::Init();
		//Title_Menu::Init();	

		Notify(ORBIS_TOOLBOX_NOTIFY);
		
		return 0;
	}

	int module_stop()
	{
		klog("!! BYE !!\n");

		Settings_Menu::Term();
		System_Monitor::Term();
		//Title_Menu::Term();

		sceKernelSleep(4);

		return 0;
	}

	void _start()
	{

	}
}