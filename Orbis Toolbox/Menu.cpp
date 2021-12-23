#include "Common.h"
#include "Menu.h"

#include "Settings_Menu.h"
#include "Debug_Features.h"
#include "Game_Overlay.h"
#include "Build_Overlay.h"
#include "Config.h"
#include "LncUtil.h"
#include "Daemons.h"

std::map<char*, MenuOption*>* Menu::Options;
bool Menu::Auto_Load_Settings;

void Menu::Init()
{
	Options = new std::map<char*, MenuOption*>();

	/*
		★Orbis Toolbox
	*/

	// Power Options
	Add_Option("id_reload_shellui", []() -> void { Notify("%s", __FUNCTION__); })->Visible = false;
	Add_Option("id_reboot", []() -> void { LncUtil::SystemReboot(); });
	Add_Option("id_shutdown", []() -> void { LncUtil::SystemShutdown(LncUtil::None); });
	Add_Option("id_suspend", []() -> void { LncUtil::SystemShutdown(LncUtil::Eap); });
	 
	// Note: Package Installer does not need to be done here
	//		 because of the fact its managed by the system.

	// Daemon Manager
	Add_Option("id_daemons", nullptr, nullptr, []() -> void {
		
		int fd;
		OrbisKernelStat stats;
		char* Dent_Buffer;
		OrbisKernelDirents *dent;
		int bpos;

		//Open a file descriptor on the directory where daemons are stored.
		fd = sceKernelOpen(DAEMON_DIR, 0, 0511);
		if (fd)
		{
			//Get the size of the directory and allocate space to read the contents.
			sceKernelFstat(fd, &stats);
			Dent_Buffer = (char*)malloc((size_t)stats.st_blksize);

			
			//Read the directory contents and if the number of byte sread returned less than or equal to zero return.
			int nread = sceKernelGetdents(fd, Dent_Buffer, (size_t)stats.st_blksize);
			if (nread <= 0)
				goto End;

			//Loop through all the directory contents by position in the buffer insuring we dont go over the number of read bytes.
			for (bpos = 0; bpos < nread;) 
			{
				//dent is our curent directory.
				dent = (OrbisKernelDirents*) (Dent_Buffer + bpos);

				//Find any daemons that arent system. Making sure the type is directory and its name doesnt contain NPXS.
				if (dent->d_type == DT_DIR && !strstr(dent->d_name, "NPXS") && !strstr(dent->d_name, ".") && !strstr(dent->d_name, PAYLOAD_DAEMON))
					Add_Daemon(dent->d_name);
					
				//Increase the position we are going to read by the size of the current directory entry.
				bpos += dent->d_reclen;
			}

			//make sure to close file descriptor when we are done.
			sceKernelClose(fd);
		}

	End:
		free(Dent_Buffer);

	});

	Add_Option("id_option_daemon_refresh", []() -> void {

		klog("id_option_daemon_refresh\n");

		int fd;
		OrbisKernelStat stats;
		char* Dent_Buffer;
		OrbisKernelDirents *dent;
		int bpos;

		//Open a file descriptor on the directory where daemons are stored.
		fd = sceKernelOpen(DAEMON_DIR, 0, 0511);
		if (fd)
		{
			//Get the size of the directory and allocate space to read the contents.
			sceKernelFstat(fd, &stats);
			Dent_Buffer = (char*)malloc((size_t)stats.st_blksize);


			//Read the directory contents and if the number of byte sread returned less than or equal to zero return.
			int nread = sceKernelGetdents(fd, Dent_Buffer, (size_t)stats.st_blksize);
			if (nread <= 0)
				goto End;

			//Loop through all the directory contents by position in the buffer insuring we dont go over the number of read bytes.
			for (bpos = 0; bpos < nread;)
			{
				//dent is our curent directory.
				dent = (OrbisKernelDirents*)(Dent_Buffer + bpos);

				//Find any daemons that arent system. Making sure the type is directory and its name doesnt contain NPXS.
				if (dent->d_type == DT_DIR && !strstr(dent->d_name, "NPXS") && !strstr(dent->d_name, ".") && !strstr(dent->d_name, PAYLOAD_DAEMON)) 
				{
					Remove_Daemon(dent->d_name);
					Add_Daemon(dent->d_name);
				}

				//Increase the position we are going to read by the size of the current directory entry.
				bpos += dent->d_reclen;
			}

			//make sure to close file descriptor when we are done.
			sceKernelClose(fd);
		}

	End:
		free(Dent_Buffer);

	});

	// ShellUI Plugin Manager
	Add_Option("id_plugins")->Visible = false;

	// Payload Loader
	Add_Option("id_payloads", nullptr, nullptr, []() -> void {
		


	})->Visible = false;

	Add_Option("id_Custom_Loader", []() -> void {

		if (Is_Daemon_Running(PAYLOAD_DAEMON))
		{
			if (!Stop_Daemon(PAYLOAD_DAEMON) && !Start_Daemon(PAYLOAD_DAEMON))
				Notify("Payload Loader: Failed to start Payload Daemon.");
		}
		else
		{
			if (!Start_Daemon(PAYLOAD_DAEMON))
				Notify("Payload Loader: Failed to start Payload Daemon.");
		}

	});

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

			Debug_Feature::DebugTitleIdLabel::Update();
			Debug_Feature::DevkitPanel::Update();
			UI::Utilities::ReloadItemList();
			Build_Overlay::Update();
			Game_Overlay::Update_Location();
			Game_Overlay::Update();

			Notify("Orbis Toolbox: Loaded Settings Successfully!");
		}
		else
			Notify("Orbis Toolbox: Failed to Load Settings...");
	});
	Add_Option("id_save_settings", []() -> void { Config::Write(SETTIN_DIR) ? Notify("Orbis Toolbox: Saved Settings Successfully!") : Notify("Orbis Toolbox: Failed to Save Settings..."); });


	Add_Option("id_internal_debug_settings", []() -> void{ Mono::Invoke<void>(Mono::App_exe, Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.DebugSystem", "DebugMenu"), nullptr, "Show"); });
}

void Menu::Term()
{
	//TODO: clear menu map
}