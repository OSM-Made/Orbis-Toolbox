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
	Add_Option("id_overlay_cpu", &Game_Overlay::Show_CPU_Temp, Type_Boolean);
	Add_Option("id_overlay_soc", &Game_Overlay::Show_SOC_Temp, Type_Boolean);

	//Debug Overlay
	Add_Option("id_overlay_dbg", &Build_Overlay::Draw, Type_Boolean, Build_Overlay::Update);


#ifdef ORBIS_TOOLBOX_DEBUG
	
	/*Add_Option("id_test", []() -> void {
		Label* CPU_Temp = nullptr;
		MonoClass* Scene = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "Scene");
		MonoClass* LayerManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppSystem", "LayerManager");
		MonoClass* ContainerScene = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "ContainerScene");

		MonoObject* Game = Mono::Invoke<MonoObject*>(Mono::App_exe, LayerManager, nullptr, "FindContainerSceneByPath", Mono::New_String("Game"));

		Game_Widget = new Widget();
		Game_Widget->Instance = Mono::Get_Property<MonoObject*>(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "Scene", Game, "RootWidget");

		CPU_Temp = new Label("CPUTEMP", 1910.0f, 10.0f, "CPU TEMP: 60C", 20, Label::fsNormal, Label::fwMedium, Label::vTop, Label::hCenter, 1.0f, 1.0f, 1.0f, 1.0f);
		Game_Widget->Append_Child("CPUTEMP", CPU_Temp);*
		
		Notify("Test Button Pressed.");
	});
	Add_Option("id_test2", Test_Func);

	Add_Option("id_Test_List", &Drawing_Style, Type_String, []() -> void {

		MonoClass* Scene = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "Scene");
		MonoClass* LayerManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppSystem", "LayerManager");
		MonoClass* ContainerScene = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "ContainerScene");

		MonoObject* Game = Mono::Invoke<MonoObject*>(Mono::App_exe, LayerManager, nullptr, "FindContainerSceneByPath", Mono::New_String("Game"));

		if(!Game_Widget)
			Game_Widget = new Widget();

		Game_Widget->Instance = Mono::Get_Property<MonoObject*>(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "Scene", Game, "RootWidget");

		Label* CPU_Temp = (Label*)Game_Widget->Get_Child("CPUTEMP");

		//Gheto but works till i can fix alignment lol
		if (!strcmp(Drawing_Style, "Left")) 
			CPU_Temp->Set_Location(10.0f, 10.0f);
		else if (!strcmp(Drawing_Style, "Right")) 
			CPU_Temp->Set_Location(1910.0f - CPU_Temp->Get_Text_Width(), 0);
		else if (!strcmp(Drawing_Style, "Center")) 
			CPU_Temp->Set_Location(1920.0f / 2.0f - (CPU_Temp->Get_Text_Width() / 2), 0);

		Notify("Test List Selection. %s", Drawing_Style);
	});*/
#else
	Add_Option("id_test2", []() -> void { });
	(*Options)["id_test"].Visible = false;
#endif
}

void Menu::Term()
{

}