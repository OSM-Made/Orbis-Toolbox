#include "Common.h"
#include "Settings_Menu.h"
#include "System_Monitor.h"

#include <orbis/LncUtil.h>

extern "C"
{
	int module_start()
	{
		klog("!! Hello World !!\n");

		Mono::Init();

		char Buffer[PATH_MAX];
		sprintf(Buffer, "/%s/common/lib/libSceSystemService.sprx", sceKernelGetFsSandboxRandomWord());
		int SystemServiceHandle = sceKernelLoadStartModule(Buffer, NULL, nullptr, NULL, nullptr, nullptr);

		klog("SystemServiceHandle: %i\n", SystemServiceHandle);

		int (*_sceLncUtilInitialize)();
		sceKernelDlsym(SystemServiceHandle, "sceLncUtilInitialize", (void*)&_sceLncUtilInitialize);

		int(*_sceLncUtilLaunchApp)();
		sceKernelDlsym(SystemServiceHandle, "sceLncUtilLaunchApp", (void*)&_sceLncUtilLaunchApp);

		int(*_sceLncUtilGetAppId)();
		sceKernelDlsym(SystemServiceHandle, "sceLncUtilGetAppId", (void*)&_sceLncUtilGetAppId);

		int(*_sceLncUtilKillAppWithReason)();
		sceKernelDlsym(SystemServiceHandle, "sceLncUtilKillAppWithReason", (void*)&_sceLncUtilKillAppWithReason);

		klog("sceLncUtilInitialize: 0x%llX\nsceLncUtilLaunchApp: 0x%llX\nsceLncUtilGetAppId: 0x%llX\nsceLncUtilKillAppWithReason: 0x%llX\n", 
			_sceLncUtilInitialize, _sceLncUtilLaunchApp, _sceLncUtilGetAppId, _sceLncUtilKillAppWithReason);

		//UI::Utilities::SetVersionString("5.05 OSM's Cool Firmware");
		System_Monitor::Init();
		Settings_Menu::Init();
		//Title_Menu::Init();	

		Notify(ORBIS_TOOLBOX_NOTIFY);

		return 0;
	}

	int module_stop()
	{
		klog("!! BYE !!\n");

		Notify("Orbis Toolbox: Unloading...");

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