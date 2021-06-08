#include "Common.h"
#include "GamePad.h"

bool GamePad::IsDown(int button)
{
	return Mono::Invoke<bool>(Mono::App_exe, Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.DebugSystem", "KeyMonitorTask"), NULL, "IsButtonDown", button);
}