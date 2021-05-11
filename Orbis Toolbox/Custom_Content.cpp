#include "Common.h"
#include "UI.h"

Detour* UI::Custom_Content::Detour_ExecuteSelectQuery = nullptr;
Detour* UI::Custom_Content::Detour_ExecuteCountQuery = nullptr;
Detour* UI::Custom_Content::Detour_StartDebugSettings = nullptr;
Detour* UI::Custom_Content::Detour_GetIconPath = nullptr;

bool UI::Custom_Content::Show_App_Home;
bool UI::Custom_Content::Show_Debug_Settings;

MonoObject* UI::Custom_Content::ExecuteSelectQuery_Hook(MonoObject* Instance, int offset, int limit)
{
	//System.Collections.Generic List
	MonoClass* List = Mono::Get_Class(Mono::mscorlib, "System.Collections.Generic", "List`1");

	MonoObject* List_Instance = Detour_ExecuteSelectQuery->Stub<MonoObject*>(Instance, offset, limit);
	if (Mono::Get_Field<int>(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemAccessor", Instance, "exclusionFilterTypeAppHome") == 0)
	{
		if (Show_Debug_Settings)
			Mono::Invoke<void>(Mono::Accessor_Db, List, List_Instance, "Insert", 0, Utilities::NewAppBrowseItem("NPXS20993", "★Orbis Toolbox"));

		if (Show_App_Home)
		{
			Mono::Invoke<void>(Mono::Accessor_Db, List, List_Instance, "Insert", 0, Utilities::NewAppBrowseItem("NPXS29998", "★APP_HOME(data)"));
			Mono::Invoke<void>(Mono::Accessor_Db, List, List_Instance, "Insert", 0, Utilities::NewAppBrowseItem("NPXS29999", "★APP_HOME(host)"));
		}

	}

	return List_Instance;
}

int UI::Custom_Content::ExecuteCountQuery_Hook(MonoObject* Instance)
{
	int Count = Detour_ExecuteCountQuery->Stub<int>(Instance);

	if (Mono::Get_Field<int>(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemAccessor", Instance, "exclusionFilterTypeAppHome") == 0)
	{
		if (Show_Debug_Settings)
			Count += 1;

		if (Show_App_Home)
			Count += 2;
	}

	return Count;
}

void UI::Custom_Content::StartDebugSettings_Hook(MonoObject* Instance)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	MonoClass* SettingsApplication = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI", "SettingsApplication");
	Mono::Invoke<void>(Mono::App_exe, UIManager, Mono::Get_Field<MonoObject*>(SettingsApplication, Instance, "uiManager"), "Push", Mono::New_String("orbis_toolbox.xml"), Mono::New_String("id_orbis_toolbox"), 3);
}

MonoString* GetTexture(const char* texId)
{
	// /Application/resource/Sce.Vsh.ShellUI.Base.rco
	return Mono::New_String("cxml://BasePlugin/%s", texId);
}

MonoString* UI::Custom_Content::GetIconPath_Hook(MonoObject* item, bool withTheme)
{
	MonoString* IconPath = Detour_GetIconPath->Stub<MonoString*>(item, withTheme);

	char* TitleId = mono_string_to_utf8(Mono::Get_Property<MonoString*>(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemLite", item, "TitleId"));
	if (!strcmp(TitleId, "NPXS29999"))
		return GetTexture("tex_app_home");
	else if (!strcmp(TitleId, "NPXS29998"))
		return GetTexture("tex_app_home_data");
	else if (!strcmp(TitleId, "NPXS20993"))
		return GetTexture("tex_debug_settings");
	else
		return IconPath;
}

void UI::Custom_Content::Init()
{
	Detour_ExecuteSelectQuery = new Detour();
	Detour_ExecuteCountQuery = new Detour();
	Detour_StartDebugSettings = new Detour();
	Detour_GetIconPath = new Detour();

	Detour_ExecuteSelectQuery->DetourMethod(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemAccessor", "ExecuteSelectQuery", 2, (void*)ExecuteSelectQuery_Hook, 15);
	Detour_ExecuteCountQuery->DetourMethod(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemAccessor", "ExecuteCountQuery", 0, (void*)ExecuteCountQuery_Hook, 15);
	Detour_StartDebugSettings->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI", "SettingsApplication", "StartDebugSettings", 0, (void*)StartDebugSettings_Hook, 14);
	Detour_GetIconPath->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI.Library", "AppBrowseItemMethodExteneder", "GetIconPath", 2, (void*)GetIconPath_Hook, 16);
}

void UI::Custom_Content::Term()
{
	delete Detour_ExecuteSelectQuery;
	delete Detour_ExecuteCountQuery;
	delete Detour_StartDebugSettings;
	delete Detour_GetIconPath;
}