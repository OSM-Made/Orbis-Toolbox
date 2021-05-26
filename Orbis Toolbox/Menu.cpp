#include "Common.h"
#include "Menu.h"

#include "Settings_Menu.h"
#include "Debug_Features.h"
#include "LncUtil.h"
#include "Game_Overlay.h"
#include "Build_Overlay.h"
#include "Config.h"

std::map<const char*, MenuOption>* Menu::Options;
bool Menu::Auto_Load_Settings;
char Test[0x100] = { "Stopped" };

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
	Add_Option("id_message");

	Add_Option("id_ORBS30000", &Test, Type_String, []() -> void { Notify("Test");  }, []() -> void { Notify("Test");  });
	Add_Option("id_OFTP00001", []() -> void { 

		int AppId = LncUtil::GetAppId("OFTP00001");
		if (AppId > 0)
		{
			LncUtil::KillApp(AppId);

			AppId = LncUtil::GetAppId("OFTP00001");
			if (AppId < 0)
			{
				UI::Utilities::Set_Value("id_OFTP00001", "Stopped");
				UI::Utilities::ResetMenuItem("id_OFTP00001");
			}
		}
		else
		{
			char Args;
			LncUtil::LaunchAppParam p = { sizeof(LncUtil::LaunchAppParam), -1, 0, 0, LncUtil::Flag_None };
			LncUtil::LaunchApp("OFTP00001", &Args, 0, &p);

			//LaunchAppParam p = { sizeof(LaunchAppParam), -1, 0, 0, LncUtilFlag::LncUtilFlagNone };
			//sceLncUtilLaunchApp("OFTP00001", 0, &p);

			AppId = LncUtil::GetAppId("OFTP00001");
			if (AppId > 0)
			{
				UI::Utilities::Set_Value("id_OFTP00001", "Running");
				UI::Utilities::ResetMenuItem("id_OFTP00001");
			}
		}
		Notify("Test");  

	});

	// Daemon Manager
	Add_Option("id_daemons", nullptr, nullptr, []() -> void {
		
		/*int fd = sceKernelOpen(DAEMON_DIR, 0, 0511);

		if (fd)
		{
			//OrbisKernelStat stats;
			//sceKernelFstat(fd, &stats);

			//char* Dent_Buffer = (char*)malloc((size_t)stats.st_blksize);

			char buf[1024];
			OrbisKernelDirents *dent;
			int bpos;

			int nread = sceKernelGetdents(fd, buf, 1024);

			if (nread <= 0)
				return;

			for (bpos = 0; bpos < nread;) 
			{
				dent = (OrbisKernelDirents*) (buf + bpos);

				//Find any daemons that arent system. Making sure the type is directory and its name doesnt contain NPXS.
				if (dent->d_type == DT_DIR && !strstr(dent->d_name, "NPXS") && !strstr(dent->d_name, "."))
				{
					klog("%s\n", dent->d_name);
					if (!strcmp(dent->d_name, "OFTP00001")) 
					{*/
						UI::Utilities::AddMenuItem(UI::Utilities::ElementData("id_OFTP00001", "Orbis FTP", "A simple FTP Daemon that has read write privlage to anywhere.", "file://system/vsh/app/OFTP00001/sce_sys/icon0.png"));

						int AppId = LncUtil::GetAppId("OFTP00001");
						klog("%i\n", AppId);

						if(AppId > 0)
							UI::Utilities::Set_Value("id_OFTP00001", "Running");
						else
							UI::Utilities::Set_Value("id_OFTP00001", "Stopped");
					/*}
				}

				bpos += dent->d_reclen;
			}

			sceKernelClose(fd);
		}*/
		(*Options)["id_message"].Visible = false;
		UI::Utilities::ResetMenuItem("id_message");

	});
	(*Options)["id_daemons"].Visible = false;

	// ShellUI Plugin Manager
	Add_Option("id_plugins");
	(*Options)["id_plugins"].Visible = false;

	// Payload Loader
	Add_Option("id_payloads");
	(*Options)["id_payloads"].Visible = false;

	// Note: System settings does not need to be done here
	//		 because of the fact its managed by the system.

	//Orbis Toolbox Settings
	Add_Option("id_load_cfg_on_start", &Auto_Load_Settings, Type_Boolean);
	Add_Option("id_system_disp_titleid", &Debug_Feature::DebugTitleIdLabel::ShowLabels, Type_Boolean, Debug_Feature::DebugTitleIdLabel::Update);
	Add_Option("id_system_disp_devkit_panel", &Debug_Feature::DevkitPanel::ShowPanel, Type_Boolean, Debug_Feature::DevkitPanel::Update);
	Add_Option("id_system_disp_debug_settings_panel", &Debug_Feature::Custom_Content::Show_Debug_Settings, Type_Boolean, UI::Utilities::ReloadItemList);
	Add_Option("id_system_disp_app_home_panel", &Debug_Feature::Custom_Content::Show_App_Home, Type_Boolean, UI::Utilities::ReloadItemList);

	//Debug Overlay
	Add_Option("id_overlay_dbg", &Build_Overlay::Draw, Type_Boolean, Build_Overlay::Update);

	//Game Overlay
	Add_Option("id_overlay_loc", &Game_Overlay::Location, Type_String, Game_Overlay::Update_Location);
	Add_Option("id_overlay_cpu_usage", &Game_Overlay::Show_CPU_Usage, Type_Boolean, Game_Overlay::Update);
	Add_Option("id_overlay_thr_count", &Game_Overlay::Show_Thread_Count, Type_Boolean, Game_Overlay::Update);
	Add_Option("id_overlay_ram", &Game_Overlay::Show_ram, Type_Boolean, Game_Overlay::Update);
	Add_Option("id_overlay_vram", &Game_Overlay::Show_vram, Type_Boolean, Game_Overlay::Update);
	Add_Option("id_overlay_cpu", &Game_Overlay::Show_CPU_Temp, Type_Boolean, Game_Overlay::Update);
	Add_Option("id_overlay_soc", &Game_Overlay::Show_SOC_Temp, Type_Boolean, Game_Overlay::Update);
	Add_Option("id_load_settings", []() -> void { 

		if (Config::Parse(SETTIN_DIR))
		{
			UI::Utilities::ResetMenuItem("id_load_cfg_on_start");
			UI::Utilities::ResetMenuItem("id_system_disp_titleid");
			UI::Utilities::ResetMenuItem("id_system_disp_devkit_panel");
			UI::Utilities::ResetMenuItem("id_system_disp_debug_settings_panel");
			UI::Utilities::ResetMenuItem("id_system_disp_app_home_panel");
			UI::Utilities::ResetMenuItem("id_overlay_dbg");

			Game_Overlay::Update_Location();
			Game_Overlay::Update();

			Notify("Orbis Toolbox: Loaded Settings Sucessfully!");
		}
		else
			Notify("Orbis Toolbox: Failed to Load Settings...");
	});
	Add_Option("id_save_settings", []() -> void { Config::Write(SETTIN_DIR) ? Notify("Orbis Toolbox: Saved Settings Sucessfully!") : Notify("Orbis Toolbox: Failed to Save Settings..."); });


#ifdef ORBIS_TOOLBOX_DEBUG
	Add_Option("id_test", []() -> void {});
#else
	Add_Option("id_test", []() -> void { });
	(*Options)["id_test"].Visible = false;
#endif
}

void Menu::Term()
{

}