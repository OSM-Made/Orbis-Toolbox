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