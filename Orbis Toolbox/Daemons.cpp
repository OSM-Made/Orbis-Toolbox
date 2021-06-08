#include "Common.h"
#include "Daemons.h"

#include "SysfileUtilWrapper.h"
#include "LncUtil.h"

bool Start_Daemon(char* TitleId)
{
	if (!Is_Daemon_Running(TitleId))
	{
		LncUtil::LaunchAppParam p = { sizeof(LncUtil::LaunchAppParam), -1, 0, 0, LncUtil::Flag_None };
		LncUtil::LaunchApp(TitleId, 0, 0, &p);

		if (!Is_Daemon_Running(TitleId))
			return false;
	}

	return true;
}

bool Stop_Daemon(char* TitleId)
{
	int AppId = LncUtil::GetAppId(TitleId);
	if (AppId > 0)
	{
		LncUtil::KillApp(AppId);

		if (Is_Daemon_Running(TitleId))
			return false;
	}

	return true;
}

bool Is_Daemon_Running(char* TitleId)
{
	return (LncUtil::GetAppId(TitleId) > 0);
}

/*
	Will start or stop a Daemon depending on its state.

	TitleId - A string representing the Daemons title Index usually in form of XXXX#####
	Restart - If the Daemon is already running will stop and restart it.
*/
bool Start_Stop_Daemon(char* TitleId, bool Restart)
{
	int AppId = LncUtil::GetAppId(TitleId);
	if (AppId > 0) 
	{
		LncUtil::KillApp(AppId);

		if ((LncUtil::GetAppId(TitleId) <= 0) && Restart)
			return Start_Stop_Daemon(TitleId, false);
		else if (LncUtil::GetAppId(TitleId) <= 0)
			return true;
		else
			return false;
	}
	else
	{
		LncUtil::LaunchAppParam p = { sizeof(LncUtil::LaunchAppParam), -1, 0, 0, LncUtil::Flag_None };
		LncUtil::LaunchApp(TitleId, 0, 0, &p);

		return (LncUtil::GetAppId(TitleId) > 0);
	}
}

void Add_Daemon(char* dent)
{
	char TitleId[10];
	char Id_Name[0x100];
	char Icon_Path[PATH_MAX];
	char SFO_Path[PATH_MAX];

	strcpy(TitleId, dent);
	sprintf(Id_Name, "id_%s", TitleId);
	sprintf(Icon_Path, "file://system/vsh/app/%s/sce_sys/icon0.png", TitleId);
	sprintf(SFO_Path, "/system/vsh/app/%s/sce_sys/param.sfo", TitleId);

	//Adds a custom button to the current drawing stack with the name and desc. of the daemon from the param.sfo
	UI::Utilities::AddMenuItem(UI::Utilities::ElementData(Id_Name, SysfileUtilWrapper::GetTitle(SFO_Path), SysfileUtilWrapper::GetDescription(SFO_Path), Icon_Path));

	//Remove Menu Option if already Exists.
	if (Menu::Has_Option(Id_Name))
		Menu::Remove_Option(Id_Name);

	//Add Menu Option with call back to load Daemon.
	Menu::Add_Option(Id_Name, [TitleId, Id_Name]() -> void {

		int AppId = LncUtil::GetAppId(TitleId);
		if (AppId > 0) //App is Currently Running.
		{
			UI::Utilities::Set_Value(Id_Name, "Stopping");
			UI::Utilities::ResetMenuItem(Id_Name);

			//Kill the app.
			LncUtil::KillApp(AppId);

			//Check to see if it worked.
			UI::Utilities::Set_Value(Id_Name, (LncUtil::GetAppId(TitleId) > 0) ? "Running" : "Stopped");
			UI::Utilities::ResetMenuItem(Id_Name);
		}
		else
		{
			UI::Utilities::Set_Value(Id_Name, "Starting");
			UI::Utilities::ResetMenuItem(Id_Name);

			LncUtil::LaunchAppParam p = { sizeof(LncUtil::LaunchAppParam), -1, 0, 0, LncUtil::Flag_None };
			LncUtil::LaunchApp(TitleId, 0, 0, &p);

			//Check to see if it worked.
			UI::Utilities::Set_Value(Id_Name, (LncUtil::GetAppId(TitleId) > 0) ? "Running" : "Stopped");
			UI::Utilities::ResetMenuItem(Id_Name);
		}

	});

	//Shows the current status of the daemon.
	UI::Utilities::Set_Value(Id_Name, (LncUtil::GetAppId(TitleId) > 0) ? "Running" : "Stopped");
	UI::Utilities::ResetMenuItem(Id_Name);
}

void Remove_Daemon(char* dent)
{
	char Id_Name[0x100];
	sprintf(Id_Name, "id_%s", dent);

	UI::Utilities::RemoveMenuItem(Id_Name);
	Menu::Remove_Option(Id_Name);
}