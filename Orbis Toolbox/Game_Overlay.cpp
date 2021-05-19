#include "Common.h"
#include "Game_Overlay.h"
#include "CPU_Monitor.h"

float Game_Overlay::X, Game_Overlay::Y;
bool Game_Overlay::Show_CPU_Usage;
bool Game_Overlay::Show_Thread_Count;
bool Game_Overlay::Show_CPU_Temp = false;
bool Game_Overlay::Show_SOC_Temp = false;
char Game_Overlay::Location[0x100] = { "Left" };

float Game_Overlay::Text_Height = 0.0f;
Widget* Game_Overlay::Game_Widget = nullptr;
std::map<const char*, CALL_BACK_TYPE>* Game_Overlay::Updater;
bool Game_Overlay::Shutdown = false;

/*
	Init_Overlay(const char* Name, CALL_BACK_TYPE_D)
		This will push each of our labels made and their call backs for data updates
		to a std::map. Currently it has issues with ordering **Needs refactor**.
*/

void Game_Overlay::Init_Overlay(const char* Name, CALL_BACK_TYPE_D)
{
	Label* Temp = new Label(Name, 10.0f, 10.0f, "", 20, Label::fsNormal, Label::fwMedium, Label::vBottom, Label::hLeft, 1.0f, 1.0f, 1.0f, 1.0f);
	Game_Widget->Append_Child(Name, Temp);
	Text_Height = Temp->Get_Text_Height();

	Updater->insert(Updater->begin(), std::make_pair(Name, CallBack));
}

/*
	Update_Location()
		This will update the global vars for X and Y depending on where the user
		decides to draw the overlay which is stored in the text Location.

		Currenttly supporting Left, Right and Center. May Impliment a vertical
		location to this in the future.
*/

void Game_Overlay::Update_Location()
{
	if (!strcmp(Location, "Left")) 
	{
		X = 10.0f, Y = 5.0f;

		for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->Set_Alignment(Label::vTop, Label::hLeft);
		}
	}
	else if (!strcmp(Location, "Right"))
	{
		X = UI::Utilities::ScreenWidth() - 10.0f, Y = 5.0f;

		for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->Set_Alignment(Label::vTop, Label::hRight);
		}
	}
	else if (!strcmp(Location, "Center"))
	{
		X = UI::Utilities::ScreenWidth() / 2.0f, Y = 5.0f;

		for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->Set_Alignment(Label::vTop, Label::hCenter);
		}
	}

}

/*
	OnRender()
		Anything called in this function will be called in the main mono render
		loop. 

		Checks for Game_Widget pointer and boolean Shutdown for safety.

		Here we will iterate through all of the overlays supported and decide 
		which to draw and their location.
*/

void Game_Overlay::OnRender()
{
	if (!Game_Widget || Shutdown)
		return;

	for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
	{
		Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
		int Order = it->second(Instance);
		if (Order)
		{
			Instance->Set_Location(X, Y + ((Order - 1) * 25.0f));
			Instance->Set_Colour(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
			Instance->Set_Colour(1.0f, 1.0f, 1.0f, 0.0f);
	}
		
}

void Game_Overlay::Init()
{
	//Get Relevent Classes.
	MonoClass* LayerManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppSystem", "LayerManager");
	MonoClass* ContainerScene = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "ContainerScene");

	//Initialize Game Scene widget
	Game_Widget = new Widget();
	MonoObject* Game = Mono::Invoke<MonoObject*>(Mono::App_exe, LayerManager, nullptr, "FindContainerSceneByPath", Mono::New_String("Game"));
	Game_Widget->Instance = Mono::Get_Property<MonoObject*>(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "Scene", Game, "RootWidget");

	//Set location to left.
	strcpy(Location, "Left");
	X = 10.0f, Y = 5.0f;

	//Init map.
	Updater = new std::map<const char*, CALL_BACK_TYPE>();
	Show_CPU_Temp = true;
	Show_SOC_Temp = true;

	int Order = 1;
	//Initialize call back for updating overlay types.
	Init_Overlay("CPULABEL", [](Label* Instance) -> int {

		if (Show_CPU_Temp)
		{
			//Update temp and set colour as visible.
			int Temp = 0;
			sceKernelGetCpuTemperature(&Temp);
			Instance->Set_Text("CPU Temp: %i C", Temp);

			return 6;
		}
		else
			return 0;

	});

	Init_Overlay("SOCLABEL", [](Label* Instance) -> int {

		if (Show_SOC_Temp)
		{
			//Update temp and set colour as visible.
			int Temp = 0;
			sceKernelGetSocSensorTemperature(0, &Temp);
			Instance->Set_Text("SOC Temp: %i C", Temp);

			return 5;
		}
		else 
			return 0;

	});

	Init_Overlay("THREADCOUNT", [](Label* Instance) -> int {

		Instance->Set_Text("Thread Count: %i", CPU_Monitor::Thread_Count);
		return 2;

	});

	Init_Overlay("CPUUSAGE", [](Label* Instance) -> int {

		//Update temp and set colour as visible.
		Instance->Set_Text("CPU Usage: %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%%", 
			CPU_Monitor::Usage[0], CPU_Monitor::Usage[1], CPU_Monitor::Usage[2], CPU_Monitor::Usage[3], 
			CPU_Monitor::Usage[4], CPU_Monitor::Usage[5], CPU_Monitor::Usage[6], CPU_Monitor::Usage[7]);

		return 1;
	});

	Init_Overlay("RAMUSAGE", [](Label* Instance) -> int {

		int cpuUsed, cpuTotal, gpuUsed, gpuTotal;
		Get_Page_Table_Stats(&cpuUsed, &cpuTotal, &gpuUsed, &gpuTotal);
		Instance->Set_Text("RAM: %u MB / %u MB", cpuUsed, cpuTotal);
		return 3;

	});

	Init_Overlay("VRAMUSAGE", [](Label* Instance) -> int {

		int cpuUsed, cpuTotal, gpuUsed, gpuTotal;
		Get_Page_Table_Stats(&cpuUsed, &cpuTotal, &gpuUsed, &gpuTotal);
		Instance->Set_Text("VRAM:  %u MB / %u MB", gpuUsed, gpuTotal);
		return 4;

	});
}

void Game_Overlay::Term()
{
	Shutdown = true;

	if (Game_Widget)
	{
		for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
			Game_Widget->Remove_Child(it->first);

		delete Game_Widget;
	}
}