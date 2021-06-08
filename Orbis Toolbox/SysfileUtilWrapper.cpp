#include "Common.h"
#include "SysfileUtilWrapper.h"

char* SysfileUtilWrapper::GetString(const char* FilePath, const char* Key, unsigned int Size)
{
	int fd = sceKernelOpen(FilePath, 0, 0511);
	if (!fd)
	{
		klog("File doesnt exist %s\n", FilePath);
		return NULL;
	}
	else
	{
		OrbisKernelStat stats;
		sceKernelFstat(fd, &stats);

		char* Buffer = (char*)malloc((size_t)stats.st_blksize);
		sceKernelRead(fd, Buffer, (size_t)stats.st_blksize);

		sceKernelClose(fd);

		if (!strstr(Buffer, Key))
		{
			klog("File \"%s\" doesnt have the key \"%s\"", FilePath, Key);
			return NULL;
		}

		MonoClass* SysfileUtilWrapper_Util = Mono::Get_Class(Mono::SysfileUtilWrapper, "Sce.Vsh", "SysfileUtilWrapper/Util");

		MonoString* str = Mono::Invoke<MonoString*>(Mono::SysfileUtilWrapper, SysfileUtilWrapper_Util, nullptr, "GetString", Mono::New_String(FilePath), Mono::New_String(Key), Size);

		if (str)
			return mono_string_to_utf8(str);
		else
			return NULL;
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