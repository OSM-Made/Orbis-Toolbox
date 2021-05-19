#pragma once
#include "Widget.h"
#include "Common.h"

#define CALL_BACK_TYPE int(*)(Label*)
#define CALL_BACK_TYPE_D int(*CallBack)(Label*)

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

	static std::map<const char*, CALL_BACK_TYPE>* Updater;
	static bool Shutdown;

	static void Init_Overlay(const char* Name, CALL_BACK_TYPE_D);
};
