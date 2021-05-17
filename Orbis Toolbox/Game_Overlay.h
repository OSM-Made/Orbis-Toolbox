#pragma once
#include "Widget.h"
#include "Common.h"

class Game_Overlay
{
public:
	static bool Show_CPU_Temp;
	static bool Show_SOC_Temp;
	static char Location[0x100];

	static void Update_Location();
	static void OnRender();

	static void Init();
	static void Term();

private:
	static float X, Y;
	static float Text_Height;
	static Widget* Game_Widget;

	static std::map<const char*, void(*)(int*, Label*)>* Updater;
	static bool Shutdown;

	static void Init_Overlay(const char* Name, void(*CallBack)(int*, Label*));
};
