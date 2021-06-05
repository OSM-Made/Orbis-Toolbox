#include "Common.h"
#include "Settings_Menu.h"
#include "System_Monitor.h"

#include <orbis/LncUtil.h>

bool ResolveAddr(int Handle, const char* Function_Name, uint64_t* Function_Address)
{
	uint64_t Address = 0;
	int result = sceKernelDlsym(Handle, Function_Name, Function_Address);
	klog("ResolveAddr(%i, \"%s\") -> 0x%llX, 0x%llX\n", Handle, Function_Name, result, *Function_Address);
	return (result == 0);
}

extern "C"
{
	int module_start()
	{
		klog("!! Hello World !!\n");

		Mono::Init();

		//TODO: Add buton combo that if detected here will cancel loading.
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