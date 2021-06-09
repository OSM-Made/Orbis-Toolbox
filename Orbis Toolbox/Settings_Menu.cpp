#include "Common.h"
#include "Settings_Menu.h"

#include "UI.h"
#include "Debug_Features.h"
#include "Game_Overlay.h"
#include "Build_Overlay.h"
#include "Config.h"

//Embedded xmls
extern uint8_t settings_root[];
extern int32_t settings_root_Size;
extern uint8_t orbis_toolbox[];
extern int32_t orbis_toolbox_Size;
extern uint8_t external_hdd[];
extern int32_t external_hdd_Size;

//Detours
Detour* Settings_Menu::Detour_GetManifestResourceStream = nullptr;
Detour* Settings_Menu::Detour_OnCheckVisible = nullptr;
Detour* Settings_Menu::Detour_OnPreCreate = nullptr;
Detour* Settings_Menu::Detour_OnPageActivating = nullptr;
Detour* Settings_Menu::Detour_OnPress = nullptr;
Detour* Settings_Menu::Detour_OnRender = nullptr;

//Patches
Patcher* Settings_Menu::Patch_IsDevkit;
Patcher* Settings_Menu::Patch_IsDebugMenuEnable;
Patcher* Settings_Menu::Patch_AllowDebugMenu;
Patcher* Settings_Menu::Patch_MainThreadCheck;

/*
	GetManifestResourceStream:
		This is the method I hook that loads internal packed resources 
		from the mono UI. I intercept the uri and force it to return a 
		new memory stream of the bytes for our custom xml.
*/

uint64_t Settings_Menu::GetManifestResourceStream_Hook(uint64_t inst, MonoString* FileName)
{
	char* str = mono_string_to_utf8(FileName);
	//klog("****\nFileName: %s\n****\n", str);

	if (!strcmp(str, "Sce.Vsh.ShellUI.src.Sce.Vsh.ShellUI.Settings.Plugins.SettingsRoot.data.settings_root.xml"))
		return (uint64_t)UI::Utilities::MemoryStream(settings_root, settings_root_Size);
	else if (!strcmp(str, "Sce.Vsh.ShellUI.src.Sce.Vsh.ShellUI.Settings.Plugins.orbis_toolbox.xml"))
		return (uint64_t)UI::Utilities::MemoryStream(orbis_toolbox, orbis_toolbox_Size);
	else if (!strcmp(str, "Sce.Vsh.ShellUI.src.Sce.Vsh.ShellUI.Settings.Plugins.external_hdd.xml"))
		return (uint64_t)UI::Utilities::MemoryStream(external_hdd, external_hdd_Size);
	else
		return Detour_GetManifestResourceStream->Stub<uint64_t>(inst, FileName);
}

/*
	Settings Root Hooks:
		I set my custom menu as the plugin of the root settings page
		this allows me to hook all the call backs from inside this
		page and run my own code.

		Note: in the xml "plugin="settings_root_plugin""
*/

/*
	OnCheckVisible:
		This could allow us to show and hide elements on each page
		much like the id_message which is a loading symbol.
*/

void Settings_Menu::OnCheckVisible_Hook(MonoObject* Instance, MonoObject* element, MonoObject* e)
{
	if (Instance && element)
	{
		char* Id = mono_string_to_utf8(Mono::Get_Property<MonoString*>(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "SettingElement", element, "Id"));
		MenuOption* Cur = Menu::Get_Option(Id);

		if (Cur)
		{
			//Show or hide Menu Options on the fly.
			Mono::Set_Property(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "SettingElement", element, "Visible", Cur->Visible);
		}
	}
	Detour_OnCheckVisible->Stub<void>(Instance, element, e);
}

/*
	OnPreCreate:
		This Hook allows us to set values of each menu element
		depending on what is set prior. For example I use this
		to set the check boxes to match the previously selected
		values.
*/

void Settings_Menu::OnPreCreate_Hook(MonoObject* Instance, MonoObject* element, MonoObject* e)
{
	if (Instance && element)
	{
		MonoClass* SettingElement = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "SettingElement");
		char* Id = mono_string_to_utf8(Mono::Get_Property<MonoString*>(SettingElement, element, "Id"));
		MenuOption* Cur = Menu::Get_Option(Id);

		if (Cur)
		{
			//Update the shown value of the option.
			if (Cur->Type == Type_Boolean)
				Mono::Set_Property(SettingElement, element, "Value", (*(bool*)Cur->Data) ? Mono::New_String("1") : Mono::New_String("0"));
			/*else if (Cur->Type == Type_Integer)
				Mono::Set_Property(SettingElement, element, "Value", Mono::New_String(std::to_string(*(int*)Cur->Data).c_str()));
			else if (Cur->Type == Type_Float)
				Mono::Set_Property(SettingElement, element, "Value", Mono::New_String(std::to_string(*(float*)Cur->Data).c_str()));*/
			else if (Cur->Type == Type_String)
				Mono::Set_Property(SettingElement, element, "Value", Mono::New_String((const char*)Cur->Data));

			//Call the OnPreCreate call back.
			if (Cur->OnPreCreate != nullptr)
				Cur->OnPreCreate();
		}
	}
	Detour_OnPreCreate->Stub<void>(Instance, element, e);
}

/*
	OnPageActivating:
		Hooking this allows us to add custom elements when a
		page is loading like for example how I will be using
		it to parse the payloads from the HDD and display
		them to be loaded.
*/

void Settings_Menu::OnPageActivating_Hook(MonoObject* Instance, MonoObject* page, MonoObject* e)
{
	if (Instance && page)
	{
		MonoClass* SettingElement = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "SettingElement");
		char* Id = mono_string_to_utf8(Mono::Get_Property<MonoString*>(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "SettingPage", page, "Id"));

		MenuOption* Cur = Menu::Get_Option(Id);

		if (Cur)
		{
			if(Cur->OnPageActivating != nullptr)
				Cur->OnPageActivating();
		}
	}
	Detour_OnPageActivating->Stub<void>(Instance, page, e);
}

/*
	OnPress:
		This Hook allows us to catch when a element is selected
		or its value is changed.
*/

void Settings_Menu::OnPress_Hook(MonoObject* Instance, MonoObject* element, MonoObject* e)
{
	if (Instance && element)
	{
		MonoClass* SettingElement = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "SettingElement");
		char* Id = mono_string_to_utf8(Mono::Get_Property<MonoString*>(SettingElement, element, "Id"));
		char* Value = mono_string_to_utf8(Mono::Get_Property<MonoString*>(SettingElement, element, "Value"));

		if (!strcmp(Id, "_option"))
			Id = Value;

		MenuOption* Cur = Menu::Get_Option(Id);
		
		if (Cur)
		{
			//Update the local value of the option.
			if (Cur->Type == Type_Boolean)
				*(bool*)Cur->Data = (atoi(Value) >= 1);
			else if (Cur->Type == Type_Integer)
				*Cur->Data = atoi(Value);
			else if (Cur->Type == Type_Float)
				*Cur->Data = atof(Value);
			else if (Cur->Type == Type_String)
				strcpy((char*)Cur->Data, Value);

			//Call the OnPress call back.
			if (Cur->OnPress != nullptr)
				Cur->OnPress();
		}
	}

	Detour_OnPress->Stub<void>(Instance, element, e);
}

void Settings_Menu::OnRender_Hook(MonoObject* Instance)
{
	static bool Do_Once = false;
	if (!Do_Once)
	{
		Log("Init Game Overlay");
		Game_Overlay::Init();

		Log("Init Build Overlay");
		Build_Overlay::Init();

		if (Config::Read(SETTIN_DIR) && Config::Data->Auto_Load_Settings) 
		{
			Config::Parse(SETTIN_DIR);

			//Call functions to reflect update
			Debug_Feature::DebugTitleIdLabel::Update();
			Debug_Feature::DevkitPanel::Update();
			UI::Utilities::ReloadItemList();
			Build_Overlay::Update();
			Game_Overlay::Update_Location();
			Game_Overlay::Update();
		}	
		else
		{
			Build_Overlay::Draw = true;
			Build_Overlay::Update();
		}

		Do_Once = true;
	}

	Game_Overlay::OnRender();

	Detour_OnRender->Stub<void>(Instance);
}

void Settings_Menu::Log(const char* fmt, ...)
{
	char va_Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, fmt);
	vsprintf(va_Buffer, fmt, args);
	va_end(args);

	klog("[Settings Menu] %s\n", va_Buffer);
}

void Settings_Menu::Init()
{
	Log("Init");

	Log(ORBIS_TOOLBOX_BUILDSTRING);

	Config::Init();

	//Debug Settings Patch
	Patch_IsDevkit = new Patcher();
	Patch_IsDebugMenuEnable = new Patcher();
	Patch_AllowDebugMenu = new Patcher();
	Patch_MainThreadCheck = new Patcher();

	Log("Install Patches");
	Patch_IsDevkit->Install_Method_Patch(Mono::KernelSysWrapper, "Sce.Vsh", "KernelSysWrapper", "IsDevKit", 0, 0, "\x48\xc7\xc0\x01\x00\x00\x00\xC3", 8);
	Patch_IsDebugMenuEnable->Install_Method_Patch(Mono::App_exe, "Sce.Vsh.ShellUI.DebugSystem", "KeyMonitorTask", "IsDebugMenuEnable", 0, 0, "\x48\xc7\xc0\x01\x00\x00\x00\xC3", 8);
	Patch_AllowDebugMenu->Install_Method_Patch(Mono::platform_dll, "Sce.Vsh.ShellUI.Settings.Sbl", "SblWrapper", "SblRcMgrIsAllowDebugMenuForSettings", 0, 0, "\x48\xc7\xc0\x01\x00\x00\x00\xC3", 8);
	Patch_MainThreadCheck->Install_Method_Patch(Mono::PlayStation_Core, "Sce.PlayStation.Core.Runtime", "Diagnostics", "CheckRunningOnMainThread", 0, 0, "\xC3", 1);

	Debug_Feature::DevkitPanel::Init();
	Debug_Feature::DebugTitleIdLabel::Init();
	Debug_Feature::Custom_Content::Init();

	Log("Init Menu");
	Menu::Init();

	Log("Init Detours");
	Detour_GetManifestResourceStream = new Detour();
	Detour_OnCheckVisible = new Detour();
	Detour_OnPreCreate = new Detour();
	Detour_OnPageActivating = new Detour();
	Detour_OnPress = new Detour();
	Detour_OnRender = new Detour();

	Log("Detour Methods");
	Detour_GetManifestResourceStream->DetourMethod(Mono::mscorlib, "System.Reflection", "Assembly", "GetManifestResourceStream", 1, (void*)GetManifestResourceStream_Hook);
	Detour_OnCheckVisible->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.SettingsRoot", "SettingsRootHandler", "OnCheckVisible", 2, (void*)OnCheckVisible_Hook);
	Detour_OnPreCreate->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.SettingsRoot", "SettingsRootHandler", "OnPreCreate", 2, (void*)OnPreCreate_Hook);
	Detour_OnPageActivating->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.SettingsRoot", "SettingsRootHandler", "OnPageActivating", 2, (void*)OnPageActivating_Hook);
	Detour_OnPress->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.SettingsRoot", "SettingsRootHandler", "OnPress", 2, (void*)OnPress_Hook);
	Detour_OnRender->DetourMethod(Mono::UI_dll, Mono::PUI, "Application", "Update", 0, (void*)OnRender_Hook);

	Log("Init Complete");
}

void Settings_Menu::Term()
{
	Config::Term();

	Debug_Feature::DevkitPanel::Term();
	Debug_Feature::DebugTitleIdLabel::Term();
	Debug_Feature::Custom_Content::Term();

	Game_Overlay::Term();
	Build_Overlay::Term();

	//Remove Denug Settings Patch
	delete Patch_IsDevkit;
	delete Patch_IsDebugMenuEnable;
	delete Patch_AllowDebugMenu;
	delete Patch_MainThreadCheck;

	//Clean up detours
	delete Detour_GetManifestResourceStream;
	delete Detour_OnCheckVisible;
	delete Detour_OnPreCreate;
	delete Detour_OnPageActivating;
	delete Detour_OnPress;
	delete Detour_OnRender;

	//Clean up menu
	Menu::Term();
}