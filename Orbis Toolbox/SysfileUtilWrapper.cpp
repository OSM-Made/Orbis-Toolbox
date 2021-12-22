#include "Common.h"
#include "SysfileUtilWrapper.h"

void Print_Bytes(char* Bytes, size_t len)
{
	int Counter = 0;
	for (size_t n = 0; n < len; n++)
	{
		printf("%02X ", Bytes[n]);

		if (Counter >= 20)	
		{
			printf("\n");
			Counter = 0;
		}

		Counter++;
	}
	printf("\n");
}

char* SysfileUtilWrapper::GetString(const char* FilePath, const char* Key, unsigned int Size)
{
	int fd = sceKernelOpen(FilePath, 0, 0511);
	if (!fd)
	{
		klog("File doesnt exist %s\n", FilePath);
		return (char*)"";
	}
	else
	{
		MonoClass* SysfileUtilWrapper_Util = Mono::Get_Class(Mono::SysfileUtilWrapper, "Sce.Vsh", "SysfileUtilWrapper/Util");

		MonoString* str = Mono::Invoke<MonoString*>(Mono::SysfileUtilWrapper, SysfileUtilWrapper_Util, nullptr, "GetString", Mono::New_String(FilePath), Mono::New_String(Key), Size);

		if (str)
			return mono_string_to_utf8(str);
		else
			return (char*)"";
	}
}

int SysfileUtilWrapper::GetAttribute(const char* FilePath)
{
	MonoClass* SysfileUtilWrapper_Util = Mono::Get_Class(Mono::SysfileUtilWrapper, "Sce.Vsh", "SysfileUtilWrapper/Util");

	return Mono::Invoke<int>(Mono::SysfileUtilWrapper, SysfileUtilWrapper_Util, nullptr, "GetAttribute", Mono::New_String(FilePath));
}

char* SysfileUtilWrapper::GetTitleId(const char* FilePath)
{
	return GetString(FilePath, "TITLE_ID", 12U);
}

char* SysfileUtilWrapper::GetContentId(const char* FilePath)
{
	return GetString(FilePath, "CONTENT_ID", 48U);
}

char* SysfileUtilWrapper::GetTitle(const char* FilePath)
{
	return GetString(FilePath, "TITLE", 128U);
}

char* SysfileUtilWrapper::GetDescription(const char* FilePath)
{
	return GetString(FilePath, "PROVIDER", 128U);
}