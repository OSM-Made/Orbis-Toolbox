#include "Common.h"
#include "Menu.h"

#include "Settings_Menu.h"
#include "Debug_Features.h"
#include "LncUtil.h"
#include "Game_Overlay.h"
#include "Build_Overlay.h"

std::map<const char*, MenuOption>* Menu::Options;

void Menu::Init()
{
	Options = new std::map<const char*, MenuOption>();

	/*
		★Orbis Toolbox
	*/

	// Power Options PowerManager.CreateMaskScene will show the shutdown screen
	Add_Option("id_reload_shellui", []() -> void { Notify("%s", __FUNCTION__); });
	Add_Option("id_reboot", []() -> void { 
		//MonoClass* PowerManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppMain", "PowerManager");
		//Mono::Invoke<void>(Mono::App_exe, PowerManager, nullptr, "CreateMaskScene", 1, 0);
		LncUtil::SystemReboot(); 
	});
	Add_Option("id_shutdown", []() -> void {
		//MonoClass* PowerManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppMain", "PowerManager");
		//Mono::Invoke<void>(Mono::App_exe, PowerManager, nullptr, "CreateMaskScene", 2, 0);
		LncUtil::SystemShutdown(LncUtil::None); 
	});
	Add_Option("id_suspend", []() -> void { 
		//MonoClass* PowerManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppMain", "PowerManager");
		//Mono::Invoke<void>(Mono::App_exe, PowerManager, nullptr, "CreateMaskScene", 3, 0);
		LncUtil::SystemShutdown(LncUtil::Eap); 
	});
	 
	// Note: Package Installer does not need to be done here
	//		 because of the fact its managed by the system.

	// Daemon Manager


	// ShellUI Plugin Manager


	// Payload Loader


	// Note: System settings does not need to be done here
	//		 because of the fact its managed by the system.

	//Orbis Toolbox Settings
	Add_Option("id_load_cfg_on_start");
	Add_Option("id_system_disp_titleid", &Debug_Feature::DebugTitleIdLabel::ShowLabels, Type_Boolean, Debug_Feature::DebugTitleIdLabel::Update);
	Add_Option("id_system_disp_devkit_panel", &Debug_Feature::DevkitPanel::ShowPanel, Type_Boolean, Debug_Feature::DevkitPanel::Update);
	Add_Option("id_system_disp_debug_settings_panel", &Debug_Feature::Custom_Content::Show_Debug_Settings, Type_Boolean, UI::Utilities::ReloadItemList);
	Add_Option("id_system_disp_app_home_panel", &Debug_Feature::Custom_Content::Show_App_Home, Type_Boolean, UI::Utilities::ReloadItemList);

	//Overlay
	Add_Option("id_overlay_loc", &Game_Overlay::Location, Type_String, Game_Overlay::Update_Location);
	Add_Option("id_overlay_cpu_usage", &Game_Overlay::Show_CPU_Usage, Type_Boolean);
	Add_Option("id_overlay_thr_count", &Game_Overlay::Show_Thread_Count, Type_Boolean);
	Add_Option("id_overlay_cpu", &Game_Overlay::Show_CPU_Temp, Type_Boolean);
	Add_Option("id_overlay_soc", &Game_Overlay::Show_SOC_Temp, Type_Boolean);

	//Debug Overlay
	Add_Option("id_overlay_dbg", &Build_Overlay::Draw, Type_Boolean, Build_Overlay::Update);


#ifdef ORBIS_TOOLBOX_DEBUG
	
#else
	Add_Option("id_test", []() -> void { });
	(*Options)["id_test"].Visible = false;
#endif
}

void Menu::Term()
{

}