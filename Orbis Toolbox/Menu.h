#pragma once
#include "Common.h"

class MenuOption
{
public:
	const char* Id;
	enum
	{
		Type_Boolean,
		Type_Integer,
		Type_Float,
		Type_String,
	}Type;
	union
	{
		bool Boolean;
		int Integer;
		float Float;
		MonoString* String;
	}Value;
	void(*OnCheckVisible)(MonoObject* Instance, MonoObject* element, MonoObject* e);
	void(*OnPreCreate)(MonoObject* Instance, MonoObject* element, MonoObject* e);
	void(*OnPageActivating)(MonoObject* Instance, MonoObject* page, MonoObject* e);
	void(*OnPress)(MonoObject* Instance, MonoObject* element, MonoObject* e);

	MenuOption();
	~MenuOption();

private:

};

MenuOption::MenuOption()
{
}

MenuOption::~MenuOption()
{
}

class Menu
{
public:
	Menu();
	~Menu();

private:

};

MenuOption* Add_Menu_Option(const char* Option_Id)
{
	MenuOption* Temp = new MenuOption();

}

Menu::Menu()
{
}

Menu::~Menu()
{
}