#include "Common.h"
#include "UI_Utilities.h"

char* UI::Utilities::Get_Version_String()
{
	MonoClass* SystemSoftwareVersionInfo = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppSystem", "SystemSoftwareVersionInfo");
 	return mono_string_to_utf8(Mono::Get_Property<MonoString*>(SystemSoftwareVersionInfo, Mono::Get_Instance(SystemSoftwareVersionInfo, "Instance"), "DisplayVersion"));
}

void UI::Utilities::SetVersionString(const char* str)
{
	MonoClass* SystemSoftwareVersionInfo = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppSystem", "SystemSoftwareVersionInfo");
	Mono::Set_Property(SystemSoftwareVersionInfo, Mono::Get_Instance(SystemSoftwareVersionInfo, "Instance"), "DisplayVersion", Mono::New_String(str));
}

void UI::Utilities::ReloadItemList()
{
	MonoClass* ContentsAreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "ContentsAreaManager");
	MonoClass* ContentsList = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "ContentsList");

	MonoObject* m_scene = Mono::Get_Field<MonoObject*>(ContentsAreaManager, Mono::Get_Instance(ContentsAreaManager, "Instance"), "m_scene");
	MonoArray* m_contentsList = Mono::Get_Field<MonoArray*>(Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "ContentAreaScene"), m_scene, "m_contentsList");
	MonoObject* m_contentsList_0 = mono_array_get(m_contentsList, MonoObject*, 0);

	if (m_contentsList_0)
		Mono::Invoke<void>(Mono::App_exe, ContentsList, m_contentsList_0, "ReloadItemSource");
}

MonoObject* UI::Utilities::AppBrowseItem(const char* TitleId, const char* TitleName)
{
	MonoObject* Instance = Mono::New_Object(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItem");
	mono_runtime_object_init(Instance);

	Mono::Set_Property(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemLite", Instance, "TitleId", Mono::New_String(TitleId));
	Mono::Set_Property(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemBase", Instance, "TitleName", Mono::New_String(TitleName));
	Mono::Set_Property(Mono::Accessor_Db, "Sce.Vsh.Accessor.Db", "AppBrowseItemBase", Instance, "MetaDataPath", 0);

	return Instance;
}

MonoObject* UI::Utilities::Get_Top_Scene()
{
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	return Mono::Invoke<MonoObject*>(Mono::App_exe, AreaManager, Mono::Get_Instance(AreaManager, "Instance"), "GetTopScene");
}

MonoObject* UI::Utilities::Get_root_Widget()
{
	return Mono::Get_Property<MonoObject*>(Mono::UI_dll, Mono::PUI_UI2, "Scene", Get_Top_Scene(), "RootWidget");
}

MonoObject* UI::Utilities::Adjust_Content(int AlignOrientation, float PaddingLeft, float PaddingRight, float PaddingTop, float PaddingBottom)
{
	MonoClass* AdjustContent = Mono::Get_Class(Mono::UI_dll, Mono::PUI_UI2, "AdjustContent");

	//Allocates memory for our new instance of a class.
	MonoObject* AdjustContent_Instance = Mono::New_Object(AdjustContent);

	//Call the default no param constructor.
	mono_runtime_object_init(AdjustContent_Instance);

	//Add Properties.
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "AlignOrientation", AlignOrientation);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingLeft", PaddingLeft);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingRight", PaddingRight);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingTop", PaddingTop);
	Mono::Set_Property(AdjustContent, AdjustContent_Instance, "PaddingBottom", PaddingBottom);

	return AdjustContent_Instance;
}

MonoObject* UI::Utilities::Fit_To_Children()
{
	MonoClass* FitToChildren = Mono::Get_Class(Mono::UI_dll, Mono::PUI_UI2, "FitToChildren");

	MonoObject* FitToChildren_Instance = Mono::New_Object(FitToChildren);
	mono_runtime_object_init(FitToChildren_Instance);

	return FitToChildren_Instance;
}

MonoObject* UI::Utilities::IUFont(int size, int style, int weight)
{
	//Somewhere between 6.72 and 7.02 UIFont was moved to UI2.
	MonoClass* UIFont = Mono::Get_Class(Mono::UI_dll, (Mono::Software_Version <= 672) ? Mono::PUI : Mono::PUI_UI2, "UIFont");

	//Allocates memory for our new instance of a class.
	MonoObject* UIFont_Instance = Mono::New_Object(UIFont);
	Mono::Invoke<void>(Mono::App_exe, UIFont, (MonoObject*)mono_object_unbox(UIFont_Instance), ".ctor", size, style, weight);

	return (MonoObject*)mono_object_unbox(UIFont_Instance);
}

MonoObject* UI::Utilities::MemoryStream(void* Buffer, int Buffer_Size)
{
	MonoArray* Array = Mono::New_Array(mono_get_byte_class(), Buffer_Size);
	char* Array_addr = mono_array_addr_with_size(Array, sizeof(char), 0);
	memcpy(Array_addr, Buffer, Buffer_Size);

	MonoClass* MemoryStream = Mono::Get_Class(Mono::mscorlib, "System.IO", "MemoryStream");
	MonoObject* MemoryStream_Instance = Mono::New_Object(MemoryStream);
	Mono::Invoke<void>(Mono::mscorlib, MemoryStream, MemoryStream_Instance, ".ctor", Array, true);

	return MemoryStream_Instance;
}

void UI::Utilities::ResetMenuItem(const char* Menu)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	Mono::Invoke<void>(Mono::App_exe, UIManager, Mono::Get_Instance(UIManager, "Instance"), "ResetMenuItem", Mono::New_String(Menu));
}

void UI::Utilities::RemoveMenuItem(const char* Menu)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	Mono::Invoke<void>(Mono::App_exe, UIManager, Mono::Get_Instance(UIManager, "Instance"), "RemoveMenuItem", Mono::New_String(Menu));
}

void UI::Utilities::AddMenuItem(MonoObject* ElementData)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	Mono::Invoke<void>(Mono::App_exe, UIManager, Mono::Get_Instance(UIManager, "Instance"), "AddMenuItem", ElementData, Mono::New_String(""));
}

MonoObject* UI::Utilities::GetElement(const char* Id)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	return Mono::Invoke<MonoObject*>(Mono::App_exe, UIManager, Mono::Get_Instance(UIManager, "Instance"), "GetElement", Mono::New_String(Id));
}

void UI::Utilities::Set_Value(const char* Id, const char* Value)
{
	MonoClass* SettingElement = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "SettingElement");

	MonoObject* Elem = GetElement(Id);
	if(Elem)
		Mono::Set_Property(SettingElement, Elem, "Value", Mono::New_String(Value));
}

MonoObject* UI::Utilities::ElementData(const char* Id, const char* Title, const char* Title2, const char* Icon)
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

MonoObject* UI::Utilities::UIColor(float R, float G, float B, float A)
{
	MonoClass* UIColor = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UIColor");

	//Allocates memory for our new instance of a class.
	MonoObject* UIColor_Instance = Mono::New_Object(UIColor);

	//  Calling the constructor for the struct** Notice that for structs we have to unbox the
	//  Object first before calling the constructor.
	MonoObject* Real_Instance = (MonoObject*)mono_object_unbox(UIColor_Instance);
	Mono::Invoke<void>(Mono::UI_dll, UIColor, Real_Instance, ".ctor", R, G, B, A);

	return Real_Instance;
}

MonoObject* UI::Utilities::UIColor(float R, float G, float B)
{
	MonoClass* UIColor = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UIColor");

	//Allocates memory for our new instance of a class.
	MonoObject* UIColor_Instance = Mono::New_Object(UIColor);

	//  Calling the constructor for the struct** Notice that for structs we have to unbox the
	//  Object first before calling the constructor.
	MonoObject* Real_Instance = (MonoObject*)mono_object_unbox(UIColor_Instance);
	Mono::Invoke<void>(Mono::UI_dll, UIColor, Real_Instance, ".ctor", R, G, B);

	return Real_Instance;
}

float UI::Utilities::ScreenHeight()
{
	MonoClass* UISystem = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UISystem");
	return Mono::Get_Property<int>(UISystem, nullptr, "ScreenHeight");
}

float UI::Utilities::ScreenWidth()
{
	MonoClass* UISystem = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UISystem");
	return Mono::Get_Property<int>(UISystem, nullptr, "ScreenWidth");
}

//Sce.Vsh.ShellUI.Library OptionMenuFactory IsAppRunning
bool UI::Utilities::IsAppRunning(const char* TitleId)
{
	MonoClass* ApplicationMonitor = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.AppSystem", "ApplicationMonitor");

	if (ApplicationMonitor == nullptr)
	{
		klog("IsAppRunning: ApplicationMonitor was null.");
		return false;
	}

	MonoMethod* Method = mono_class_get_method_from_name(ApplicationMonitor, "GetAppEvent", 1);

	if (!Method)
	{
		klog("Get_Address_of_Method: failed to find method \"%s\" in class \"%s\"", "GetAppEvent", "ApplicationMonitor");
		return false;
	}

	void* Args[] = { Mono::New_String(TitleId) };

	uint64_t obj = (uint64_t)mono_runtime_invoke(Method, nullptr, Args, NULL);

	//MonoObject* Res = Mono::Invoke<MonoObject*>(Mono::App_exe, ApplicationMonitor, nullptr, "GetAppEvent", Mono::New_String(TitleId));

	klog("obj = %llX\n", obj);
	return (obj != NULL);

	return true;
}