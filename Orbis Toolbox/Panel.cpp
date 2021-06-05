#include "Common.h"
#include "UI.h"
#include "Widget.h"
#include "Panel.h"

void Panel::Set_Position(float X, float Y)
{
	Mono::Set_Property(Panel_Class, Instance, "X", X);
	Mono::Set_Property(Panel_Class, Instance, "Y", Y);
}

void Panel::Set_Size(float Width, float Height)
{
	Mono::Set_Property(Panel_Class, Instance, "Width", Width);
	Mono::Set_Property(Panel_Class, Instance, "Height", Height);
}

void Panel::Set_Colour(float R, float G, float B, float A)
{
	Mono::Set_Property_Invoke(Panel_Class, Instance, "BackgroundColor", UI::Utilities::UIColor(R, G, B, A));
}

void Panel::Set_Rendering_Order(RenderingOrder Order)
{
	Mono::Set_Property(Panel_Class, Instance, "RenderingOrder", Order);
}

void Panel::Set_Layout_Rule(MonoObject* Rule)
{
	Mono::Set_Property(Panel_Class, Instance, "LayoutRule", Rule);
}

Panel::Panel(const char* Name)
{
	Panel_Class = Mono::Get_Class(Mono::UI_dll, Mono::PUI_UI2, "Panel");

	//Allocates memory for our new instance of a class.
	Instance = Mono::New_Object(Panel_Class);

	//Call Constructor.
	mono_runtime_object_init(Instance);

	//Set Panel Name
	Mono::Set_Property(Panel_Class, Instance, "Name", Mono::New_String(Name));
}

Panel::Panel(const char* Name, float X, float Y, float Width, float Height, float R, float G, float B, float A, RenderingOrder Order, MonoObject* Rule)
{
	Panel_Class = Mono::Get_Class(Mono::UI_dll, Mono::PUI_UI2, "Panel");

	//Allocates memory for our new instance of a class.
	Instance = Mono::New_Object(Panel_Class);

	//Call Constructor.
	mono_runtime_object_init(Instance);

	//Set Panel Name
	Mono::Set_Property(Panel_Class, Instance, "Name", Mono::New_String(Name));

	//Set Values
	Set_Position(X, Y);
	Set_Size(Width, Height);
	Set_Colour(R, G, B, A);
	Set_Rendering_Order(Order);
	Set_Layout_Rule(Rule);
}

Panel::~Panel()
{

}