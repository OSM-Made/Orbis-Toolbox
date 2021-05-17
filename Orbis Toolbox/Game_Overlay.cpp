#include "Common.h"
#include "Game_Overlay.h"

float Game_Overlay::X, Game_Overlay::Y;
bool Game_Overlay::Show_CPU_Temp = false;
bool Game_Overlay::Show_SOC_Temp = false;
char Game_Overlay::Location[0x100] = { "Left" };

float Game_Overlay::Text_Height = 0.0f;
Widget* Game_Overlay::Game_Widget = nullptr;
std::map<const char*, void(*)(int*, Label*)>* Game_Overlay::Updater;
bool Game_Overlay::Shutdown = false;

void Game_Overlay::Init_Overlay(const char* Name, void(*CallBack)(int*, Label*))
{
	Label* Temp = new Label(Name, 10.0f, 10.0f, "", 20, Label::fsNormal, Label::fwMedium, Label::vBottom, Label::hLeft, 1.0f, 1.0f, 1.0f, 1.0f);
	Game_Widget->Append_Child(Name, Temp);

	Updater->insert(Updater->begin(), std::make_pair(Name, CallBack));
}

void Game_Overlay::Update_Location()
{
	if (!strcmp(Location, "Left")) 
	{
		X = 10.0f, Y = 5.0f;

		for (std::map<const char*, void(*)(int*, Label*)>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->Set_Alignment(Label::vTop, Label::hLeft);
		}
	}
	else if (!strcmp(Location, "Right"))
	{
		X = UI::Utilities::ScreenWidth() - 10.0f, Y = 5.0f;

		for (std::map<const char*, void(*)(int*, Label*)>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->Set_Alignment(Label::vTop, Label::hRight);
		}
	}
	else if (!strcmp(Location, "Center"))
	{
		X = UI::Utilities::ScreenWidth() / 2.0f, Y = 5.0f;

		for (std::map<const char*, void(*)(int*, Label*)>::iterator it = Updater->begin(); it != Updater->end(); it++)
		{
			Label* Instance = (Label*)Game_Widget->Get_Child(it->first);
			Instance->Set_Alignment(Label::vTop, Label::hCenter);
		}
	}

}

void Game_Overlay::OnRender()
{
	if (!Game_Widget || Shutdown)
		return;

	int Count = 0;
	for (std::map<const char*, void(*)(int*, Label*)>::iterator it = Updater->begin(); it != Updater->end(); it++)
		it->second(&Count, (Label*)Game_Widget->Get_Child(it->first));
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
	Updater = new std::map<const char*, void(*)(int*, Label*)>();
	Show_CPU_Temp = true;
	Show_SOC_Temp = true;

	//Initialize call back for updating overlay types.
	Init_Overlay("CPULABEL", [](int* Count, Label* Instance) -> void {

		if (Show_CPU_Temp)
		{
			//Update temp and set colour as visible.
			int Temp = 0;
			sceKernelGetCpuTemperature(&Temp);
			Instance->Set_Text("CPU Temp: %i C", Temp);
			Instance->Set_Location(X, Y + (*Count * Instance->Get_Text_Height()));
			Instance->Set_Colour(1.0f, 1.0f, 1.0f, 1.0f);

			//Itterate Count since were drawing.
			*Count += 1;
		}
		else
			Instance->Set_Colour(1.0f, 1.0f, 1.0f, 0.0f);

	});
	Init_Overlay("SOCLABEL", [](int* Count, Label* Instance) -> void {

		if (Show_SOC_Temp)
		{
			//Update temp and set colour as visible.
			int Temp = 0;
			sceKernelGetSocSensorTemperature(0, &Temp);
			Instance->Set_Text("SOC Temp: %i C", Temp);
			Instance->Set_Location(X, Y + (*Count * Instance->Get_Text_Height()));
			Instance->Set_Colour(1.0f, 1.0f, 1.0f, 1.0f);

			//Itterate Count since were drawing.
			*Count += 1;
		}
		else
			Instance->Set_Colour(1.0f, 1.0f, 1.0f, 0.0f);

	});
}

void Game_Overlay::Term()
{
	Shutdown = true;

	if (Game_Widget)
	{
		for (std::map<const char*, void(*)(int*, Label*)>::iterator it = Updater->begin(); it != Updater->end(); it++)
			Game_Widget->Remove_Child(it->first);

		delete Game_Widget;
	}
}