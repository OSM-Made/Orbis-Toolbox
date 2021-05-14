#include "Common.h"
#include "LncUtil.h"

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