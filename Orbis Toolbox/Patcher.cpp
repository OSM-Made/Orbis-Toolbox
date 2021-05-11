#include "Common.h"
#include "Patcher.h"

void Patcher::Install_Patch(uint64_t Address, const void* Data, size_t Length)
{
	//Backup Params.
	this->Address = Address;
	this->Length = Length;

	//Set protection to all
	sceKernelMprotect((void*)Address, Length, VM_PROT_ALL);

	//Backup data.
	int res = sceKernelMmap(0, Length, VM_PROT_ALL, 0x1000 | 0x2, -1, 0, &OriginalData);
	if (res < 0)
	{
		klog("[Patcher] sceKernelMmap Failed: 0x%llX\n", res);
		return;
	}
	memcpy(OriginalData, Data, Length);

	//Write Patch.
	memcpy((void*)Address, Data, Length);

	klog("[Patcher] Install_Patch: Patch (%llX) Written Successfully!\n", Address);
}

void Patcher::Install_Method_Patch(MonoImage* Assembly_Image, const char* Namespace, const char* Klass, const char* Method, int Param_Count, size_t Offset, const void* Data, size_t Length)
{
	uint64_t Method_Address = Mono::Get_Address_of_Method(Assembly_Image, Namespace, Klass, Method, Param_Count);

	if (Method_Address == NULL)
	{
		klog("[Patcher] Install_Method_Patch: Method address returned null!\n");
		return;
	}

	Install_Patch(Method_Address + Offset, Data, Length);
}

void Patcher::Restore_Patch()
{
	if (this->OriginalData)
	{
		//Set protection to all
		sceKernelMprotect((void*)this->Address, this->Length, VM_PROT_ALL);

		//Write original Data back.
		memcpy((void*)this->Address, this->OriginalData, this->Length);

		klog("[Patcher] Restore_Patch: Patch (%llX) Restored Successfully!\n", this->Address);
	}
	else
		klog("[Patcher] Patch not installed.\n");
}

Patcher::Patcher()
{

}

Patcher::~Patcher()
{
	Restore_Patch();

	//Clean up
	sceKernelMunmap(this->OriginalData, this->Length);
}