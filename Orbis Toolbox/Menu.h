#pragma once
#include "Common.h"

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
	char Id[0x100];
	Data_Type Type;
	uint64_t* Data;
	bool Visible;
	std::function<void()> OnPreCreate;
	std::function<void()> OnPageActivating;
	std::function<void()> OnPress;

	MenuOption() { } 
	~MenuOption() { } 

private:

};

class Menu
{
public:
	template<typename Value>
	static MenuOption* Add_Option(const char* Option_Id, Value* Data, Data_Type Type, std::function<void()> OnPress = nullptr, std::function<void()> OnPreCreate = nullptr, std::function<void()> OnPageActivating = nullptr)
	{
		klog("Add_Option\n");
		MenuOption* Temp = new MenuOption();
		strcpy(Temp->Id, Option_Id);
		Temp->Data = (uint64_t*)Data;
		Temp->Type = Type;
		Temp->Visible = true;
		Temp->OnPreCreate = OnPreCreate;
		Temp->OnPageActivating = OnPageActivating;
		Temp->OnPress = OnPress;

		Options->insert(std::pair<char*, MenuOption*>(Temp->Id, Temp));

		return Temp;
	}

	static MenuOption* Add_Option(const char* Option_Id, std::function<void()> OnPress = nullptr, std::function<void()> OnPreCreate = nullptr, std::function<void()> OnPageActivating = nullptr)
	{
		klog("Add_Option\n");
		MenuOption* Temp = new MenuOption();
		strcpy(Temp->Id, Option_Id);
		Temp->Type = Type_None;
		Temp->Visible = true;
		Temp->OnPreCreate = OnPreCreate;
		Temp->OnPageActivating = OnPageActivating;
		Temp->OnPress = OnPress;

		Options->insert(std::pair<char*, MenuOption*>(Temp->Id, Temp));
		
		return Temp;
	}

	/*static bool Has_Option(const char* Option_Id)
	{

	}

	static MenuOption* Get_Option(const char* Option_Id)
	{

	}

	static void Remove_Option(const char* Option_Id)
	{

	}*/

	static std::map<char*, MenuOption*>* Options;
	static bool Auto_Load_Settings;

	static void Init();
	static void Term();

private:

};

#define DAEMON_DIR "/system/vsh/app/"
#define PLUGIN_DIR "/data/Orbis Toolbox/Plugins"
#define SETTIN_DIR "/data/Orbis Toolbox/Settings.cfg"
