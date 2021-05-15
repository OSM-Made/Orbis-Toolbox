#include "Common.h"
#include "UI.h"
#include "Widget.h"
#include "Label.h"

void Label::Set_Location(float X, float Y)
{
	Mono::Set_Property(Label_Class, Instance, "X", X);
	Mono::Set_Property(Label_Class, Instance, "Y", Y);
}

void Label::Set_Text(const char* Text)
{
	Mono::Set_Property(Label_Class, Instance, "Text", Mono::New_String(Text));
}

void Label::Set_Font(int Size, FontStyle Style, FontWeight Weight)
{
	Mono::Set_Property_Invoke(Label_Class, Instance, "Font", UI::Utilities::IUFont(Size, Style, Weight));
}

void Label::Set_Alignment(VerticalAlignment Vertical_Align, HorizontalAlignment Horizontal_Align)
{
	Mono::Set_Property(Label_Class, Instance, "VerticalAlignment", Vertical_Align);
	Mono::Set_Property(Label_Class, Instance, "HorizontalAlignment", Horizontal_Align);
}

void Label::Set_Colour(float R, float G, float B, float A)
{
	Mono::Set_Property_Invoke(Label_Class, Instance, "TextColor", UI::Utilities::UIColor(R, G, B, A));
}

float Label::Get_Text_Width()
{
	return Mono::Invoke<float>(Mono::App_exe, Label_Class, Instance, "GetTextWidth");
}

float Label::Get_Text_Height()
{
	return Mono::Invoke<float>(Mono::App_exe, Label_Class, Instance, "GetTextHeight");
}

Label::Label(const char* Name)
{
	Label_Class = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "Label");

	//Allocates memory for our new instance of a class.
	Instance = Mono::New_Object(Label_Class);

	//Call Constructor.
	mono_runtime_object_init(Instance);

	//Set Panel Name
	Mono::Set_Property(Label_Class, Instance, "Name", Mono::New_String(Name));
}

Label::Label(const char* Name, float X, float Y, const char* Text, int Size, FontStyle Style, FontWeight Weight, VerticalAlignment Vertical_Align, HorizontalAlignment Horizontal_Align, float R, float G, float B, float A)
{
	Label_Class = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI3", "Label");

	//Allocates memory for our new instance of a class.
	Instance = Mono::New_Object(Label_Class);

	//Call Constructor.
	mono_runtime_object_init(Instance);

	//Set Panel Name
	Mono::Set_Property(Label_Class, Instance, "Name", Mono::New_String(Name));

	//Set Values
	Set_Location(X, Y);
	Set_Text(Text);
	Set_Font(Size, Style, Weight);
	Set_Alignment(Vertical_Align, Horizontal_Align);
	Set_Colour(R, G, B, A);
	Mono::Set_Property(Label_Class, Instance, "FitWidthToText", true);
	Mono::Set_Property(Label_Class, Instance, "FitHeightToText", true);
}

Label::~Label()
{

}