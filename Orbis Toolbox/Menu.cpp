#include "Common.h"
#include "Menu.h"
#include "Settings_Menu.h"
#include "Debug_Features.h"

std::map<const char*, MenuOption>* Menu::Options;

void Test_Func()
{
	Settings_Menu::Init_Debug_Label();

	Notify("Test Button Pressed.");
}

void Menu::Init()
{
	Options = new std::map<const char*, MenuOption>();

	/*
		★Orbis Toolbox
	*/

	// Power Options
	Add_Option("id_reload_shellui", Power_Options::Reload_ShellUI);
	Add_Option("id_reboot", Power_Options::Reboot);
	Add_Option("id_shutdown", Power_Options::Shutdown);
	Add_Option("id_suspend", Power_Options::Suspend);

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

#ifdef ORBIS_TOOLBOX_DEBUG
	Add_Option("id_test", Test_Func);
#else
	Add_Option("id_test", Test_Func);
	(*Options)["id_test"].Visible = false;
#endif
}

void Menu::Term()
{

}