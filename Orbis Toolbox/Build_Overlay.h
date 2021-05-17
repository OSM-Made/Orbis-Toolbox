#pragma once
#include "Common.h"
#include "Widget.h"

class Build_Overlay
{
public:
	static bool Draw;

	static void Update();
	static void Init();
	static void Term();

private:
	static Widget* Root_Widget;
};
