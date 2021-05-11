#pragma once

class Patcher
{
public:
	void Install_Patch(uint64_t Address, const void* Data, size_t Length);
	void Install_Method_Patch(MonoImage* Assembly_Image, const char* Namespace, const char* Klass, const char* Method, int Param_Count, size_t Offset, const void* Data, size_t Length);
	void Restore_Patch();

	Patcher();
	~Patcher();

private:
	uint64_t Address;
	void* OriginalData;
	size_t Length;
};
