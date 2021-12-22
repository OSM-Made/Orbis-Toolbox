#include "Common.h"
#include "LncUtil.h"

int LncUtil::GetAppStatus(AppStatus* Status)
{
	MonoClass* LncUtil_Class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil");
	MonoClass* AppStatus = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil/AppStatus");

	MonoObject* AppStatus_Instance = Mono::New_Object(AppStatus);
	if (AppStatus_Instance)
	{
		Mono::Invoke<void>(Mono::platform_dll, AppStatus, (MonoObject*)mono_object_unbox(AppStatus_Instance), ".ctor", Status->appId, Status->launchRequestAppId, Status->appType);

		int res = Mono::Invoke<int>(Mono::platform_dll, LncUtil_Class, nullptr, "GetAppStatus", mono_object_unbox(AppStatus_Instance));

		memcpy(Status, (void*)mono_object_unbox(AppStatus_Instance), sizeof(AppStatus));

		return res;
	}

	return 0;
}

//int SuspendApp(int appId, Flag flag = Flag_None);
//int ResumeApp(int appId, Flag flag = Flag_None);

//static int SetControllerFocus(int appId);
//static int SetAppFocus(int appId, Flag flag = Flag_None);

int LncUtil::GetAppId(const char* titleId)
{
	MonoClass* LncUtil_Class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil");
	return Mono::Invoke<int>(Mono::platform_dll, LncUtil_Class, nullptr, "GetAppId", Mono::New_String(titleId));
}

int LncUtil::LaunchApp(const char* titleId, char* args, int argsSize, LaunchAppParam* param)
{
	MonoClass* LncUtil_Class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil");

	//
	// Init Launch Param
	//
	MonoClass* LaunchAppParam_class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil/LaunchAppParam");

	MonoObject* LaunchAppParam_Instance = Mono::New_Object(LaunchAppParam_class);
	MonoObject* LaunchAppParam_Instance_real = (MonoObject*)mono_object_unbox(LaunchAppParam_Instance);

	Mono::Set_Field(LaunchAppParam_Instance, "size", param->size);
	Mono::Set_Field(LaunchAppParam_Instance, "userId", param->userId);
	Mono::Set_Field(LaunchAppParam_Instance, "appAttr", param->appAttr);
	Mono::Set_Field(LaunchAppParam_Instance, "enableCrashReport", param->enableCrashReport);
	Mono::Set_Field(LaunchAppParam_Instance, "checkFlag", param->checkFlag);

	//LaunchAppParam* p = (LaunchAppParam*)mono_object_unbox(LaunchAppParam_Instance);
	//memcpy(p, param, sizeof(LaunchAppParam));

	//
	// Init Byte Class.
	//
	MonoArray* Array = Mono::New_Array(mono_get_byte_class(), argsSize);
	char* Array_addr = mono_array_addr_with_size(Array, sizeof(char), 0);

	if(args && argsSize > 0)
		memcpy(Array_addr, args, argsSize);

	klog("Calling Launch...\n");

	return Mono::Invoke<int>(Mono::platform_dll, LncUtil_Class, nullptr, "LaunchApp", Mono::New_String(titleId), Array, argsSize, LaunchAppParam_Instance_real);
}

int LncUtil::KillApp(int appId, int userId)
{
	MonoClass* LncUtil_Class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil");
	return Mono::Invoke<int>(Mono::platform_dll, LncUtil_Class, nullptr, "KillApp", appId, userId, 0, 0);
}

//static int ForceKillApp(int appId, int userId = -1);
//static int KillLocalProcess(int appId, int appLocalPid);

void LncUtil::SystemShutdown(Boot flag)
{
	MonoClass* LncUtil_Class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil");
	Mono::Invoke<int>(Mono::platform_dll, LncUtil_Class, nullptr, "SystemShutdown", flag);
}

void LncUtil::SystemReboot()
{
	MonoClass* LncUtil_Class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil");
	Mono::Invoke<int>(Mono::platform_dll, LncUtil_Class, nullptr, "SystemReboot");
}

void LncUtil::SystemSuspend()
{
	MonoClass* LncUtil_Class = Mono::Get_Class(Mono::platform_dll, "Sce.Vsh.ShellUI.Lnc", "LncUtil");
	Mono::Invoke<int>(Mono::platform_dll, LncUtil_Class, nullptr, "SystemSuspend");
}

