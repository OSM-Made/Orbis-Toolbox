#include "Common.h"
#include "Power_Options.h"
#include "LncUtil.h"

void Power_Options::Reload_ShellUI()
{
	Notify("%s", __FUNCTION__);
}

void Power_Options::Reboot()
{
	LncUtil::SystemReboot();
}

void Power_Options::Shutdown()
{
	LncUtil::SystemShutdown(LncUtil::None);
}

void Power_Options::Suspend()
{
	LncUtil::SystemShutdown(LncUtil::Eap);
}