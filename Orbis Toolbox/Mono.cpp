#include "Common.h"
#include "Mono.h"

MonoDomain* Mono::Root_Domain;
MonoImage* Mono::PlayStation_Core;
MonoImage* Mono::App_exe;
MonoImage* Mono::platform_dll;
MonoImage* Mono::UI_dll;
MonoImage* Mono::KernelSysWrapper;
MonoImage* Mono::mscorlib;
MonoImage* Mono::Accessor_Db;
MonoImage* Mono::Vsh_Lx;
MonoImage* Mono::SysfileUtilWrapper;

int Mono::Software_Version;
const char* Mono::PUI;
const char* Mono::PUI_UI2;
const char* Mono::PUI_UI3;

bool Mono::Init()
{
	MonoLog("Init");

	Root_Domain = mono_get_root_domain();
	mono_thread_attach(Root_Domain);

	if (Root_Domain == nullptr)
	{
		MonoLog("Mono: Failed to get root domain.");
		return false;
	}

	MonoLog("Get Images");

	UI_dll = Get_Image("/%s/common/lib/Sce.PlayStation.PUI.dll", sceKernelGetFsSandboxRandomWord());
	if (!UI_dll)
	{
		klog("Using Highlevel.UI2...\n");
		UI_dll = Get_Image("/%s/common/lib/Sce.PlayStation.HighLevel.UI2.dll", sceKernelGetFsSandboxRandomWord());
	}
	else
		klog("Using PUI...\n");

	App_exe = Get_Image("/app0/psm/Application/app.exe");
	platform_dll = Get_Image("/app0/psm/Application/platform.dll");
	PlayStation_Core = Get_Image("/%s/common/lib/Sce.PlayStation.Core.dll", sceKernelGetFsSandboxRandomWord());
	KernelSysWrapper = Get_Image("/%s/common/lib/Sce.Vsh.KernelSysWrapper.dll", sceKernelGetFsSandboxRandomWord());
	mscorlib = Get_Image("/%s/common/lib/mscorlib.dll", sceKernelGetFsSandboxRandomWord());
	Accessor_Db = Get_Image("/%s/common/lib/Sce.Vsh.Accessor.Db.dll", sceKernelGetFsSandboxRandomWord());
	Vsh_Lx = Get_Image("/%s/common/lib/Sce.Vsh.Lx.dll", sceKernelGetFsSandboxRandomWord());
	SysfileUtilWrapper = Get_Image("/%s/common/lib/Sce.Vsh.SysfileUtilWrapper.dll", sceKernelGetFsSandboxRandomWord());

	SceKernelSystemSwVersion Version;
	Version.Size = sizeof(SceKernelSystemSwVersion);
	sceKernelGetSystemSwVersion(&Version);
	char Version_Short[] = { Version.info[1], Version.info[3], Version.info[4] };
	Software_Version = atoi(Version_Short);
	klog("Software Version: %s %i\n", Version.info, Software_Version);

	switch (Software_Version)
	{
	default:
		klog("Unsuported Software Version!! \"%s\"(%i)\n", Version.info, Software_Version);
		break;

	case 505:
		PUI = "Sce.PlayStation.HighLevel.UI2";
		PUI_UI2 = "Sce.PlayStation.HighLevel.UI2";
		PUI_UI3 = "Sce.PlayStation.HighLevel.UI2";
		break;

	case 672:
	case 702:
	case 755:
	case 900:
		PUI = "Sce.PlayStation.PUI";
		PUI_UI2 = "Sce.PlayStation.PUI.UI2";
		PUI_UI3 = "Sce.PlayStation.PUI.UI3";
		break;
	}

	MonoLog("Init Complete");

	return true;
}

void Mono::MonoLog(const char* fmt, ...)
{
	char va_Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, fmt);
	vsprintf(va_Buffer, fmt, args);
	va_end(args);

	klog("[Mono] %s\n", va_Buffer);
}

MonoImage* Mono::Get_Image(const char* Assembly_Name, ...)
{
	char va_Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, Assembly_Name);
	vsprintf(va_Buffer, Assembly_Name, args);
	va_end(args);

	MonoAssembly* Assembly = mono_domain_assembly_open(Root_Domain, va_Buffer);
	if (Assembly == nullptr)
	{
		MonoLog("GetImage: Failed to open \"%s\" assembly.", va_Buffer);
		return nullptr;
	}

	MonoImage* Assembly_Image = mono_assembly_get_image(Assembly);
	if (Assembly_Image == nullptr)
	{
		MonoLog("GetImage: Failed to open \"%s\" Image.", va_Buffer);
		return nullptr;
	}

	return Assembly_Image;
}

//
// Classes
//

MonoClass* Mono::Get_Class(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name)
{
	MonoClass* klass = mono_class_from_name(Assembly_Image, Namespace, Class_Name);
	if (klass == nullptr)
		MonoLog("Get_Class: Failed to open \"%s\" class from \"%s\" Namespace.", Class_Name, Namespace);

	return klass;
}

//
// Objects
//

MonoObject* Mono::New_Object(MonoClass* Klass)
{
	if (Klass == nullptr)
	{
		MonoLog("New_Object: Klass pointer was null.");
		return nullptr;
	}

	MonoObject* Obj = mono_object_new(Root_Domain, Klass);

	if (Obj == nullptr)
		MonoLog("New_Object: Failed to Create new object \"%s\".", Klass->name);

	return Obj;
}

MonoObject* Mono::New_Object(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name)
{
	MonoClass* klass = Get_Class(Assembly_Image, Namespace, Class_Name);

	if (klass != nullptr)
		return New_Object(klass);
	else
		return nullptr;
}

MonoString* Mono::New_String(const char* str, ...)
{
	char va_Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, str);
	vsprintf(va_Buffer, str, args);
	va_end(args);

	return mono_string_new(Root_Domain, va_Buffer);
}

MonoArray* Mono::New_Array(MonoClass* eclass, int size)
{
	return mono_array_new(Root_Domain, eclass, size);
}

//
// Methods
//

uint64_t Mono::Get_Address_of_Method(MonoImage* Assembly_Image, const char* Name_Space, const char* Class_Name, const char* Method_Name, int Param_Count)
{
	MonoClass* klass = Get_Class(Assembly_Image, Name_Space, Class_Name);

	if (!klass)
	{
		MonoLog("Get_Address_of_Method: failed to open class \"%s\" in namespace \"%s\"", Class_Name, Name_Space);
		return 0;
	}

	MonoMethod* Method = mono_class_get_method_from_name(klass, Method_Name, Param_Count);

	if (!Method)
	{
		MonoLog("Get_Address_of_Method: failed to find method \"%s\" in class \"%s\"", Method_Name, Class_Name);
		return 0;
	}

	return (uint64_t)mono_aot_get_method(Root_Domain, Method);
}

uint64_t Mono::Get_Address_of_Method(MonoImage* Assembly_Image, MonoClass* klass, const char* Method_Name, int Param_Count)
{
	if (!klass)
	{
		MonoLog("Get_Address_of_Method: klass was null.");
		return 0;
	}

	MonoMethod* Method = mono_class_get_method_from_name(klass, Method_Name, Param_Count);

	if (!Method)
	{
		MonoLog("Get_Address_of_Method: failed to find method \"%s\" in class \"%s\"", Method_Name, klass->name);
		return 0;
	}

	return (uint64_t)mono_aot_get_method(mono_get_root_domain(), Method);
}

//
// Properties
//

MonoObject* Mono::Get_Instance(MonoClass* Klass, const char* Instance)
{
	if (Klass == nullptr)
	{
		MonoLog("Get_Instance: Klass was null.");
		return nullptr;
	}

	MonoProperty* inst_prop = mono_class_get_property_from_name(Klass, Instance);

	if (inst_prop == nullptr)
	{
		MonoLog("Failed to find Instance property \"%s\" in Class \"%s\".", Instance, Klass->name);
		return nullptr;
	}

	MonoMethod* inst_get_method = mono_property_get_get_method(inst_prop);

	if (inst_get_method == nullptr)
	{
		MonoLog("Failed to find get method for \"%s\" in Class \"%s\".", Instance, Klass->name);
		return nullptr;
	}

	MonoObject* inst = mono_runtime_invoke(inst_get_method, 0, 0, 0);

	if (inst == nullptr)
	{
		MonoLog("Failed to find get Instance \"%s\" in Class \"%s\".", Instance, Klass->name);
		return nullptr;
	}

	return inst;
}

MonoObject* Mono::Get_Instance(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name, const char* Instance)
{
	return Get_Instance(Get_Class(Assembly_Image, Namespace, Class_Name), Instance);
}
