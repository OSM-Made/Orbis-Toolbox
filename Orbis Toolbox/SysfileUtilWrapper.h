#pragma once

class SysfileUtilWrapper
{
public:
	static char* GetString(const char* FilePath, const char* Key, unsigned int Size);

	static int GetAttribute(const char* FilePath);
	static char* GetTitleId(const char* FilePath);
	static char* GetContentId(const char* FilePath);
	static char* GetTitle(const char* FilePath);
	static char* GetDescription(const char* FilePath);

private:

};