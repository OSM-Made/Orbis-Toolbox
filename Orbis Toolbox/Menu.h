#pragma once
#include "Common.h"

#include "Power_Options.h"

enum Data_Type
{
	Type_None,
	Type_Boolean,
	Type_Integer,
	Type_Float,
	Type_String,
};

class MenuOption
{
public:
	Data_Type Type;
	union
	{
		bool* Boolean;
		int* Integer;
		float* Float;
		char* String;
	}Data;
	bool Visible;
	void(*OnPreCreate)();
	void(*OnPageActivating)();
	void(*OnPress)();

	MenuOption() { } 
	~MenuOption() { } 

private:

};

class Menu
{
public:
	template<typename Value>
	static MenuOption Add_Option(const char* Option_Id, Value* Data, Data_Type Type, void(*OnPress)() = nullptr, void(*OnPreCreate)() = nullptr, void(*OnPageActivating)() = nullptr)
	{
		MenuOption Temp;
		Temp.Data = Data;
		Temp.Type = Type;
		Temp.Visible = true;
		Temp.OnPreCreate = OnPreCreate;
		Temp.OnPageActivating = OnPageActivating;
		Temp.OnPress = OnPress;

		Options->insert(std::pair<const char*, MenuOption>(Option_Id, Temp));

		return Temp;
	}

	static MenuOption Add_Option(const char* Option_Id, void(*OnPress)() = nullptr, void(*OnPreCreate)() = nullptr, void(*OnPageActivating)() = nullptr)
	{
		MenuOption Temp;
		Temp.Type = Type_None;
		Temp.Visible = true;
		Temp.OnPreCreate = OnPreCreate;
		Temp.OnPageActivating = OnPageActivating;
		Temp.OnPress = OnPress;

		Options->insert(std::pair<const char*, MenuOption>(Option_Id, Temp));
		
		return Temp;
	}

	static std::map<const char*, MenuOption>* Options;

	static void Init();
	static void Term();

private:

};
