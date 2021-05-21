#include "Common.h"
#include "Game_Overlay.h"
#include "CPU_Monitor.h"

float Game_Overlay::X, Game_Overlay::Y;
bool Game_Overlay::Show_CPU_Usage = false;
bool Game_Overlay::Show_Thread_Count = false;
bool Game_Overlay::Show_ram = false;
bool Game_Overlay::Show_vram = false;
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
			Instance->hAlign = Label::hLeft;
		}
	}	
	else if (!strcmp(Location, "Right")) 
	{
		X = UI::Utilities::ScreenWidth() - 10.0f, Y = 5.0f;
		for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->hAlign = Label::hRight;
		}
	}
	else if (!strcmp(Location, "Center"))
	{
		X = UI::Utilities::ScreenWidth() / 2.0f, Y = 5.0f;
		for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->hAlign = Label::hCenter;
		}
	}

	Update();
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

	// Static but allows us to decide the order while retaining labels
	// are pushed up depending on which ones are enabled.
	static int Waiter = 0;

	if (Waiter <= 0)
	{
		for (std::map<const char*, CALL_BACK_TYPE>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			it->second(Instance);
		}

		Waiter = 20;
	}
	else
		Waiter--;
}

void inline Game_Overlay::Update_Label(int* Location, const char* Name)
{
	Label* Instance = (Label*)Game_Widget->Get_Child(Name);
	if ((*Updater)[Name](Instance))
	{
		/*if (!strcmp(Game_Overlay::Location, "Left"))
			Instance->Set_Location(X, Y + (*Location * 25.0f));
		else if (!strcmp(Game_Overlay::Location, "Right"))
			Instance->Set_Location(X - Instance->Get_Text_Width(), Y + (*Location * 25.0f));
		else if (!strcmp(Game_Overlay::Location, "Center"))
			Instance->Set_Location(X - (Instance->Get_Text_Width() / 2), Y + (*Location * 25.0f));*/
		Instance->Set_Location(X, Y + (*Location * 25.0f));
		Instance->Set_Colour(1.0f, 1.0f, 1.0f, 1.0f);
		*Location += 1;
	}
	else
		Instance->Set_Colour(1.0f, 1.0f, 1.0f, 0.0f);
}

void Game_Overlay::Update()
{
	klog("Update...\n");

	int Count = 0;
	Update_Label(&Count, "CPUUSAGE");
	Update_Label(&Count, "THREADCOUNT");
	Update_Label(&Count, "RAMUSAGE");
	Update_Label(&Count, "VRAMUSAGE");
	Update_Label(&Count, "CPUTEMP");
	Update_Label(&Count, "SOCTEMP");
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

	//Init map: For somereason doesnt work with out being allocated.
	Updater = new std::map<const char*, CALL_BACK_TYPE>();

	//Initialize call back for updating overlay types.
	Init_Overlay("CPUTEMP", [](Label* Instance) -> bool {

		if (Show_CPU_Temp)
		{
			int Temp = 0;
			sceKernelGetCpuTemperature(&Temp);
			Instance->Set_Text("CPU Temp: %i C", Temp);

			return true;
		}
		else
			return false;

	});

	Init_Overlay("SOCTEMP", [](Label* Instance) -> bool {

		if (Show_SOC_Temp)
		{
			int Temp = 0;
			sceKernelGetSocSensorTemperature(0, &Temp);
			Instance->Set_Text("SOC Temp: %i C", Temp);

			return true;
		}
		else
			return false;

	});

	Init_Overlay("THREADCOUNT", [](Label* Instance) -> bool {

		if (Show_Thread_Count)
		{
			Instance->Set_Text("Thread Count: %i", CPU_Monitor::Thread_Count);
		
			return true;
		}
		else
			return false;

	});

	Init_Overlay("CPUUSAGE", [](Label* Instance) -> bool {

		if (Show_CPU_Usage)
		{
			Instance->Set_Text("CPU Usage: %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%%", 
				CPU_Monitor::Usage[0], CPU_Monitor::Usage[1], CPU_Monitor::Usage[2], CPU_Monitor::Usage[3], 
				CPU_Monitor::Usage[4], CPU_Monitor::Usage[5], CPU_Monitor::Usage[6], CPU_Monitor::Usage[7]);

			return true;
		}
		else
			return false;
	});

	Init_Overlay("RAMUSAGE", [](Label* Instance) -> bool {

		if (Show_ram)
		{
			int cpuUsed, cpuTotal, gpuUsed, gpuTotal;
			Get_Page_Table_Stats(&cpuUsed, &cpuTotal, &gpuUsed, &gpuTotal);
			Instance->Set_Text("RAM: %u MB / %u MB", cpuUsed, cpuTotal);

			return true;
		}
		else
			return false;

	});

	Init_Overlay("VRAMUSAGE", [](Label* Instance) -> bool {

		if (Show_vram)
		{
			int cpuUsed, cpuTotal, gpuUsed, gpuTotal;
			Get_Page_Table_Stats(&cpuUsed, &cpuTotal, &gpuUsed, &gpuTotal);
			Instance->Set_Text("VRAM:  %u MB / %u MB", gpuUsed, gpuTotal);

			return true;
		}
		else
			return false;

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