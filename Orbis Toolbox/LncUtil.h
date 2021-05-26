#pragma once

class LncUtil
{
public:
	enum Boot
	{
		None = 0,
		Eap = 1
	};

	enum Flag : unsigned long
	{
		Flag_None = 0UL,
		SkipLaunchCheck = 1UL,
		SkipResumeCheck = 1UL,
		SkipSystemUpdateCheck = 2UL,
		RebootPatchInstall = 4UL,
		VRMode = 8UL,
		NonVRMode = 16UL
	};

	struct LaunchAppParam
	{
		unsigned int size;
		int userId;
		int appAttr;
		int enableCrashReport;
		Flag checkFlag;
	};

	enum AppType
	{
		Invalid = -1,
		Unknown,
		ShellUI,
		Daemon,
		CDLG,
		MiniApp,
		BigApp,
		ShellCore,
		ShellApp
	};

	struct AppStatus
	{
		int appId;
		int launchRequestAppId;
		char appType;
	};

	static int GetAppStatus(AppStatus* Status);
	static int SuspendApp(int appId, Flag flag = Flag_None);
	static int ResumeApp(int appId, Flag flag = Flag_None);

	static int SetControllerFocus(int appId);
	static int SetAppFocus(int appId, Flag flag = Flag_None);
	
	static int GetAppId(const char* titleId);
	static int LaunchApp(const char* titleId, char* args, int argsSize, LaunchAppParam* param);
	static int KillApp(int appId, int userId = -1);
	static int ForceKillApp(int appId, int userId = -1);
	static int KillLocalProcess(int appId, int appLocalPid);

	static void SystemShutdown(Boot flag);
	static void SystemReboot();
	static void SystemSuspend();

private:

};