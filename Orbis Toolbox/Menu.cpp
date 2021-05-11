#include "Common.h"
#include "Menu.h"

std::map<const char*, MenuOption>* Menu::Options;

void Menu::Init()
{
	Options = new std::map<const char*, MenuOption>();

	/*
		★Orbis Toolbox
	*/

	//Power Options
	Add_Option("id_reload_shellui", Power_Options::Reload_ShellUI);
	Add_Option("id_reboot", Power_Options::Reboot);
	Add_Option("id_shutdown", Power_Options::Shutdown);
	Add_Option("id_suspend", Power_Options::Suspend);
}

void Menu::Term()
{

}