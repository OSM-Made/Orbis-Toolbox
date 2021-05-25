#include "Common.h"
#include "Config.h"

#include "Settings_Menu.h"
#include "Debug_Features.h"
#include "LncUtil.h"
#include "Game_Overlay.h"
#include "Build_Overlay.h"
#include "Config.h"

/*bool Config::Parse(const char* File)
{
	//Clear the previous data.
	Config_Data.clear();

	//Check if file is already open.
	if (RFile_Handle.is_open())
	{
		RFile_Handle.close();

		klog("Failed to open File (%s) - File already open\n", File);
		return false;
	}

	//Open file.
	RFile_Handle = std::fstream(File, std::ios::in);

	//Make sure file actually opened.
	if (!RFile_Handle.is_open())
	{
		klog("Failed to open File (%s)\n", File);
		return false;
	}

	//Global Vars for parsing the config file.
	std::map<std::string, std::string> Current_Members;
	std::string Current_Line;
	std::string Current_Section;
	int Current_Line_Number = 0;

	//Loop through all lines of file.
	while (std::getline(RFile_Handle, Current_Line))
	{
		//Increment Line count.
		Current_Line_Number++;

		//If the line starts with the comment block or is an empty line ignore this line.
		if ((strstr((Current_Line.substr(0, 1)).c_str(), ";")) || Current_Line.empty()) {
			klog("Found Comment / Empty Line at %i\n", Current_Line_Number);
			continue;
		}

		//If Line is a Section Block set current Section we are in.
		if (strstr((Current_Line.substr(0, 1)).c_str(), "["))
		{
			//If were entering a new section store the previous section to the map.
			if ((Current_Section.c_str() != NULL && Current_Section.empty() == false) && Config_Data.find(Current_Section.c_str()) == Config_Data.end()) {
				klog("Storing Section \"%s\" to map.\n", Current_Section.c_str());
				Config_Data.insert(std::make_pair(Current_Section, Current_Members));
			}

			//clear the temporary stored data.
			Current_Members.clear();

			//Get the index of the open and close delimiter and set the string between them as our current section. 
			unsigned first = Current_Line.find('[') + 1;
			unsigned last = Current_Line.find(']');
			Current_Section = Current_Line.substr(first, last - first);

			klog("Section \"%s\" Found at line %i\n", Current_Section.c_str(), Current_Line_Number);

			//Go to next line to start parsing section.
			continue;
		}

		std::string Member_Name = "";
		std::string Member_Data = "";

		//Parse member name from before '=' and the data to EOL after '='.
		Member_Name = Current_Line.substr(0, Current_Line.find('='));
		Member_Data = Current_Line.substr(Current_Line.find('=') + 1, (Current_Line.length() - (Current_Line.find('=') + 1) - 1));

		//Insert member into temporary map.
		Current_Members.insert(std::make_pair(Member_Name, Member_Data));
	}

	//Store The last Section.
	if (Config_Data.find(Current_Section.c_str()) == Config_Data.end()) {
		klog("Storing Section \"%s\" to map.\n", Current_Section.c_str());
		Config_Data.insert(std::make_pair(Current_Section, Current_Members));
	}

	if (!RFile_Handle.is_open()) {
		return false;
	}

	RFile_Handle.close();

	return true;
}

bool Config::Does_Member_Exist(const char* Section, const char* Member)
{
	if (Config_Data.find(Section) == Config_Data.end())
	{
		klog("Failed to find Section \"%s\".\n", Section);
		return false;
	}

	if (Config_Data[Section].find(Member) == Config_Data[Section].end())
	{
		klog("Failed to find Member \"%s\" in Section \"%s\".\n", Member, Section);
		return false;
	}

	return true;
}

bool Config::Read_Bool(const char* Section, const char* Member)
{
	if (!Does_Member_Exist(Section, Member))
		return false;

	std::string Data = Config_Data[Section][Member];

	if (Data.compare("true") || Data.compare("yes") || Data.compare("1"))
		return true;
	else
		return false;
}

int Config::Read_Int(const char* Section, const char* Member)
{
	if (!Does_Member_Exist(Section, Member))
		return -1;

	return atoi(Config_Data[Section][Member].c_str());
}

float Config::Read_Float(const char* Section, const char* Member)
{
	if (!Does_Member_Exist(Section, Member))
		return -1.0f;

	return atof(Config_Data[Section][Member].c_str());
}

std::string Config::Read_String(const char* Section, const char* Member)
{
	if (!Does_Member_Exist(Section, Member))
		return "";

	return Config_Data[Section][Member];
}*/

Config::Data_s* Config::Data;
#define CFG_VERSION 1

bool Config::Read(const char* File)
{
	int fd = sceKernelOpen(File, SCE_KERNEL_O_RDONLY, 0511);

	if (fd)
	{
		//Reade the data then close the file handle.
		sceKernelRead(fd, (void*)Data, sizeof(Data_s));
		sceKernelClose(fd);

		//Make sure the version matches.
		if (Data->Version != CFG_VERSION)
		{
			klog("[Config] CFG Version miss match (%i != %i)...\nConfig Could be corrupt...\n", CFG_VERSION, Data->Version);

			return false;
		}

		//Could be a good idea to take a digest here to see if things have changed and compare with the digest at 0x4.


		klog("[Config] Read Config Sucessfully.\n");

		return true;
	}
	else
	{
		klog("[Config] File: %s Does not exist.\n", File);
		return false;
	}
}

bool Config::Parse(const char* File)
{
	if (Read(File))
	{
		//Parse data out.
		Menu::Auto_Load_Settings = Data->Auto_Load_Settings;
		Debug_Feature::DebugTitleIdLabel::ShowLabels = Data->Show_DebugTitleIdLabel;
		Debug_Feature::DevkitPanel::ShowPanel = Data->Show_DevkitPanel;
		Debug_Feature::Custom_Content::Show_Debug_Settings = Data->Show_Debug_Settings;
		Debug_Feature::Custom_Content::Show_App_Home = Data->Show_App_Home;

		Build_Overlay::Draw = Data->Show_Build_Overlay;

		strcpy(Game_Overlay::Location, Data->Game_Overlay_Location);
		Game_Overlay::Show_CPU_Usage = Data->Show_CPU_Usage;
		Game_Overlay::Show_Thread_Count = Data->Show_Thread_Count;
		Game_Overlay::Show_ram = Data->Show_ram;
		Game_Overlay::Show_vram = Data->Show_vram;
		Game_Overlay::Show_CPU_Temp = Data->Show_CPU_Temp;
		Game_Overlay::Show_SOC_Temp = Data->Show_SOC_Temp;

		return true;
	}

	return false;
}

bool Config::Write(const char* File)
{
	int fd = sceKernelOpen(File, SCE_KERNEL_O_CREAT | SCE_KERNEL_O_WRONLY, 0777);

	if (fd)
	{
		//Build new cfg file.
		Data->Version = CFG_VERSION;
			
		Data->Auto_Load_Settings = Menu::Auto_Load_Settings;
		Data->Show_DebugTitleIdLabel = Debug_Feature::DebugTitleIdLabel::ShowLabels;
		Data->Show_DevkitPanel = Debug_Feature::DevkitPanel::ShowPanel;
		Data->Show_Debug_Settings = Debug_Feature::Custom_Content::Show_Debug_Settings;
		Data->Show_App_Home = Debug_Feature::Custom_Content::Show_App_Home;
			
		Data->Show_Build_Overlay = Build_Overlay::Draw;
			
		strcpy(Data->Game_Overlay_Location, Game_Overlay::Location);
		Data->Show_CPU_Usage = Game_Overlay::Show_CPU_Usage;
		Data->Show_Thread_Count = Game_Overlay::Show_Thread_Count;
		Data->Show_ram = Game_Overlay::Show_ram;
		Data->Show_vram = Game_Overlay::Show_vram;
		Data->Show_CPU_Temp = Game_Overlay::Show_CPU_Temp;
		Data->Show_SOC_Temp = Game_Overlay::Show_SOC_Temp;

		//Take digest and write it.

		//Write Data
		sceKernelWrite(fd, (void*)Data, sizeof(Data_s));
		sceKernelClose(fd);

		return true;
	}
	else
	{
		klog("[Config] File: %s Does not exist & Could not be created.\n", File);
		return false;
	}
}

void Config::Init()
{
	Data = new Data_s();
}

void Config::Term()
{
	delete Data;
}