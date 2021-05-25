/*#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

class Config
{
public:
	bool Parse(const char* File);

	bool Read_Bool(const char* Section, const char* Member);
	int Read_Int(const char* Section, const char* Member);
	float Read_Float(const char* Section, const char* Member);
	std::string Read_String(const char* Section, const char* Member);

private:
	std::fstream RFile_Handle;
	std::fstream WFile_Handle;

	std::map<std::string, std::map<std::string, std::string>> Config_Data;

	bool Does_Member_Exist(const char* Section, const char* Member);
};*/
#pragma once

class Config
{
private:
	struct Data_s
	{
		int Version;
		
		bool Auto_Load_Settings;
		bool Show_DebugTitleIdLabel;
		bool Show_DevkitPanel;
		bool Show_Debug_Settings;
		bool Show_App_Home;

		bool Show_Build_Overlay;

		char Game_Overlay_Location[0x100];
		bool Show_CPU_Usage;
		bool Show_Thread_Count;
		bool Show_ram;
		bool Show_vram;
		bool Show_CPU_Temp;
		bool Show_SOC_Temp;
	};

public:
	static Data_s* Data;

	static bool Read(const char* File);
	static bool Parse(const char* File);
	static bool Write(const char* File);

	static void Init();
	static void Term();
};
