#pragma once
#include "Widget.h"

class Panel : public Widget
{
public:
	enum Orientation
	{
		Horizontal,
		Vertical
	};

	enum RenderingOrder
	{
		First = -1,
		DontCare,
		Last
	};

	void Set_Position(float X, float Y);
	void Set_Size(float Width, float Height);
	void Set_Colour(float R, float G, float B, float A);
	void Set_Rendering_Order(RenderingOrder Order);
	void Set_Layout_Rule(MonoObject* Rule);

	Panel(const char* Name);
	Panel(const char* Name, float X, float Y, float W, float H, float R, float G, float B, float A, RenderingOrder Order, MonoObject* Rule);
	~Panel();

private:
	MonoClass* Panel_Class;

};