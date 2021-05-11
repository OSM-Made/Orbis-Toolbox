#include "Common.h"
#include "Utilities.h"

/*unsigned long Syscall(unsigned int n, ...) {
	asm(".intel_syntax noprefix");
	asm("xor %rax, %rax");
	asm("mov %r10, %rcx");
	asm("syscall");
	asm("ret");
}*/

void klog(const char* fmt, ...)
{
	char Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, fmt);
	vsprintf(Buffer, fmt, args);
	va_end(args);

	sceKernelDebugOutText(0, Buffer);
}

void Notify(const char* MessageFMT, ...)
{
	NotifyBuffer Buffer;

	//Create full string from va list.
	va_list args;
	va_start(args, MessageFMT);
	vsprintf(Buffer.Message, MessageFMT, args);
	va_end(args);

	//Populate the notify buffer.
	Buffer.Type = NotifyType::NotificationRequest; //this one is just a standard one and will print what ever is stored at the buffer.Message.
	Buffer.unk3 = 0;
	Buffer.UseIconImageUri = 1; //Bool to use a custom uri.
	Buffer.TargetId = -1; //Not sure if name is correct but is always set to -1.
	strcpy(Buffer.Uri, "https://i.imgur.com/SJPIBGG.png"); //Copy the uri to the buffer.

	//From user land we can call int64_t sceKernelSendNotificationRequest(int64_t unk1, char* Buffer, size_t size, int64_t unk2) which is a libkernel import.
	sceKernelSendNotificationRequest(0, (char*)&Buffer, 3120, 0);

	//What sceKernelSendNotificationRequest is doing is opening the device "/dev/notification0" or "/dev/notification1"
	// and writing the NotifyBuffer we created to it. Somewhere in ShellUI it is read and parsed into a json which is where
	// I found some clues on how to build the buffer.
}

MonoObject* New_MemoryStream(void* Buffer, int Buffer_Size)
{
	MonoArray* Array = Mono::New_Array(mono_get_byte_class(), Buffer_Size);
	char* Array_addr = mono_array_addr_with_size(Array, sizeof(char), 0);
	memcpy(Array_addr, Buffer, Buffer_Size);

	MonoClass* MemoryStream = Mono::Get_Class(Mono::mscorlib, "System.IO", "MemoryStream");
	MonoObject* MemoryStream_Instance = Mono::New_Object(MemoryStream);
	Mono::Invoke<void>(Mono::mscorlib, MemoryStream, MemoryStream_Instance, ".ctor", Array, true);

	return MemoryStream_Instance;
}

void ResetMenuItem(const char* Menu)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	Mono::Invoke<void>(Mono::App_exe, UIManager, Mono::Get_Instance(UIManager, "Instance"), "ResetMenuItem", Mono::New_String(Menu));
}

void AddMenuItem(MonoObject* ElementData)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	Mono::Invoke<void>(Mono::App_exe, UIManager, Mono::Get_Instance(UIManager, "Instance"), "AddMenuItem", ElementData, Mono::New_String(""));
}

MonoObject* NewElementData(const char* Id, const char* Title, const char* Title2, const char* Icon)
{
	MonoClass* ButtonElementData = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "ButtonElementData");
	MonoClass* ElementData = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "ElementData");
	MonoObject* Instance = Mono::New_Object(ButtonElementData);
	mono_runtime_object_init(Instance);

	Mono::Set_Property(ElementData, Instance, "Id", Mono::New_String(Id));
	Mono::Set_Property(ElementData, Instance, "Title", Mono::New_String(Title));
	Mono::Set_Property(ElementData, Instance, "SecondTitle", Mono::New_String(Title2));
	Mono::Set_Property(ElementData, Instance, "Icon", Mono::New_String(Icon));

	return Instance;
}

MonoObject* NewUIColor(float R, float G, float B, float A)
{
	MonoClass* UIColor = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "UIColor");

	//Allocates memory for our new instance of a class.
	MonoObject* UIColor_Instance = Mono::New_Object(UIColor);

	//  Calling the constructor for the struct** Notice that for structs we have to unbox the
	//  Object first before calling the constructor.
	MonoObject* Real_Instance = (MonoObject*)mono_object_unbox(UIColor_Instance);
	Mono::Invoke<void>(Mono::Highlevel_UI2, UIColor, Real_Instance, ".ctor", R, G, B, A);

	return Real_Instance;
}