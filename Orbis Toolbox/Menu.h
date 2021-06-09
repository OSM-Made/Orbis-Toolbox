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

struct CompareFirst {
	template <typename T, typename U>
	bool operator()(T const& t, U const& u) const { return !strcmp(t.first, u.first); }
};

class Menu
{
public:
	template<typename Value>
	static MenuOption* Add_Option(const char* Option_Id, Value* Data, Data_Type Type, std::function<void()> OnPress = nullptr, std::function<void()> OnPreCreate = nullptr, std::function<void()> OnPageActivating = nullptr)
	{
		//klog("Add_Option(): %s\n", Option_Id);
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
		//klog("Add_Option(): %s\n", Option_Id);
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

	static bool Has_Option(char* Option_Id)
	{
		return std::find_if(Options->begin(), Options->end(), [Option_Id](auto a1) -> bool { return !strcmp(a1.first, Option_Id); }) != Options->end();
	}

	static MenuOption* Get_Option(char* Option_Id)
	{
		std::map<char*, MenuOption*>::iterator it = std::find_if(Options->begin(), Options->end(), [Option_Id](auto a1) -> bool { return !strcmp(a1.first, Option_Id); });
		if (it != Options->end())
			return it->second;
		else 
		{
			klog("[Menu] Get_Option(): Option \"%s\" Does not exist.\n", Option_Id);
			return nullptr;
		}
	}

	static void Remove_Option(char* Option_Id)
	{
		std::map<char*, MenuOption*>::iterator it = std::find_if(Options->begin(), Options->end(), [Option_Id](auto a1) -> bool { return !strcmp(a1.first, Option_Id); });
		if (it != Options->end())
		{
			Options->erase(it);

			klog("[Menu] Remove_Option(): Removed Option \"%s\"\n", Option_Id);
		}
		else
			klog("[Menu] Remove_Option(): Option \"%s\" Does not exist.\n", Option_Id);
	}

	static std::map<char*, MenuOption*>* Options;
	static bool Auto_Load_Settings;

	static void Init();
	static void Term();

private:

};

#define DAEMON_DIR "/system/vsh/app/"
#define PLUGIN_DIR "/user/data/Orbis Toolbox/Plugins"
#define SETTIN_DIR "/user/data/Orbis Toolbox/Settings.cfg"
#define PAYLOAD_DIR "/user/data/Orbis Toolbox/Payloads"
#define PAYLOAD_DAEMON (char*)"PLDR00000"
