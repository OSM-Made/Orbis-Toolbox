#include "Common.h"
#include "UI.h"

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
	return Mono::Get_Property<MonoObject*>(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "Scene", Get_Top_Scene(), "RootWidget");
}

MonoObject* UI::Utilities::Adjust_Content(int AlignOrientation, float PaddingLeft, float PaddingRight, float PaddingTop, float PaddingBottom)
{
	MonoClass* AdjustContent = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "AdjustContent");

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
	MonoClass* FitToChildren = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "FitToChildren");

	MonoObject* FitToChildren_Instance = Mono::New_Object(FitToChildren);
	mono_runtime_object_init(FitToChildren_Instance);

	return FitToChildren_Instance;
}

MonoObject* UI::Utilities::IUFont(int size, int style, int weight)
{
	MonoClass* UIFont = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI.UI2" : "Sce.PlayStation.HighLevel.UI2", "UIFont");

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

void UI::Utilities::AddMenuItem(MonoObject* ElementData)
{
	MonoClass* UIManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.Settings.Core", "UIManager");
	Mono::Invoke<void>(Mono::App_exe, UIManager, Mono::Get_Instance(UIManager, "Instance"), "AddMenuItem", ElementData, Mono::New_String(""));
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
	MonoClass* UIColor = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI" : "Sce.PlayStation.HighLevel.UI2", "UIColor");

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
	MonoClass* UIColor = Mono::Get_Class(Mono::UI_dll, Mono::PUI2 ? "Sce.PlayStation.PUI" : "Sce.PlayStation.HighLevel.UI2", "UIColor");

	//Allocates memory for our new instance of a class.
	MonoObject* UIColor_Instance = Mono::New_Object(UIColor);

	//  Calling the constructor for the struct** Notice that for structs we have to unbox the
	//  Object first before calling the constructor.
	MonoObject* Real_Instance = (MonoObject*)mono_object_unbox(UIColor_Instance);
	Mono::Invoke<void>(Mono::UI_dll, UIColor, Real_Instance, ".ctor", R, G, B);

	return Real_Instance;
}