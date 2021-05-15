#include "Common.h"
#include "Detour.h"
#include "Mono.h"
#include "hde64.h"

void Detour::WriteJump(void* Address, void* Destination)
{
	uint8_t JumpInstructions[] = {
	   0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, // jmp QWORD PTR[Address]
	};

	//Write the address of our hook to the instruction.
	*(uint64_t*)(JumpInstructions + 6) = (uint64_t)Destination;

	sceKernelMprotect((void*)Address, sizeof(JumpInstructions), VM_PROT_ALL);
	memcpy(Address, JumpInstructions, sizeof(JumpInstructions));
}

void* Detour::DetourFunction(uint64_t FunctionPtr, void* HookPtr)
{
	if (FunctionPtr == NULL || HookPtr == NULL)
	{
		klog("[Detour] DetourFunction: FunctionPtr or HookPtr NULL (%llX -> %llX)\n", FunctionPtr, HookPtr);
		return (void*)0;
	}
	uint32_t InstructionSize = 0;

	while (InstructionSize < 14)
	{
		hde64s hs;
		uint32_t temp = hde64_disasm((void*)(FunctionPtr + InstructionSize), &hs);

		if (hs.flags & F_ERROR)
			return (void*)0;

		InstructionSize += temp;
	}

	klog("InstructionSize: %i\n", InstructionSize);

	if (InstructionSize < 14)
	{
		klog("[Detour] DetourFunction: Hooking Requires a minimum of 14 bytes to write jump!\n");
		return (void*)0;
	}

	//Save Pointers for later
	this->FunctionPtr = (void*)FunctionPtr;
	this->HookPtr = HookPtr;

	//Set protection.
	sceKernelMprotect((void*)FunctionPtr, InstructionSize, VM_PROT_ALL);

	//Allocate Executable memory for stub and write instructions to stub and a jump back to original execution.
	this->StubSize = (InstructionSize + 14);
	int res = sceKernelMmap(0, this->StubSize, VM_PROT_ALL, 0x1000 | 0x2, -1, 0, &this->StubPtr);
	if (res < 0 || this->StubPtr == 0)
	{
		klog("[Detour] sceKernelMmap Failed: 0x%llX\n", res);
		return 0;
	}

	memcpy(StubPtr, (void*)FunctionPtr, InstructionSize);
	WriteJump((void*)((uint64_t)StubPtr + InstructionSize), (void*)(FunctionPtr + InstructionSize));

	//Write jump from function to hook.
	WriteJump((void*)FunctionPtr, HookPtr);

	klog("[Detour] DetourFunction: Detour (%llX -> %llX) Written Successfully!\n", FunctionPtr, this->HookPtr);

	return this->StubPtr;
}

void* Detour::DetourMethod(MonoImage* Assembly_Image, const char* Namespace, const char* Klass, const char* Method, int Param_Count, void* HookPtr)
{
	uint64_t Method_addr = Mono::Get_Address_of_Method(Assembly_Image, Namespace, Klass, Method, Param_Count);

	if (Method_addr == NULL)
	{
		klog("[Detour] DetourMethod: Method address returned null!\n");
		return (void*)0;
	}

	return DetourFunction(Method_addr, HookPtr);
}

void Detour::RestoreFunction()
{
	if (this->StubPtr)
	{
		sceKernelMprotect((void*)this->FunctionPtr, this->StubSize - 14, VM_PROT_ALL);
		memcpy((void*)this->FunctionPtr, this->StubPtr, this->StubSize - 14);

		klog("[Detour] RestoreFunction: (%llX) has been Restored Successfully!\n", this->FunctionPtr);
	}
}

Detour::Detour()
{

}

Detour::~Detour()
{
	RestoreFunction();

	//Clean up
	sceKernelMunmap(this->StubPtr, this->StubSize);
}