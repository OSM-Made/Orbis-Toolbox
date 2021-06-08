#pragma once
#include "Widget.h"
class Widget;

class Settings_Menu
{
private:
	//Detours
	static Detour* Detour_GetManifestResourceStream;
	static Detour* Detour_OnCheckVisible;
	static Detour* Detour_OnPreCreate;
	static Detour* Detour_OnPageActivating;
	static Detour* Detour_OnPress;
	static Detour* Detour_OnRender;

	static uint64_t GetManifestResourceStream_Hook(uint64_t inst, MonoString* FileName);
	static void OnCheckVisible_Hook(MonoObject* Instance, MonoObject* element, MonoObject* e);
	static void OnPreCreate_Hook(MonoObject* Instance, MonoObject* element, MonoObject* e);
	static void OnPageActivating_Hook(MonoObject* Instance, MonoObject* page, MonoObject* e);
	static void OnPress_Hook(MonoObject* Instance, MonoObject* element, MonoObject* e);
	static void OnRender_Hook(MonoObject* Instance);

	//Patches
	static Patcher* Patch_IsDevkit;
	static Patcher* Patch_IsDebugMenuEnable;
	static Patcher* Patch_AllowDebugMenu;
	static Patcher* Patch_MainThreadCheck;

public:
	static void Log(const char* fmt, ...);
	static void Init();
	static void Term();

};
