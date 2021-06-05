#include "Common.h"
#include "UI.h"
#include "Panel.h"
#include "Label.h"
#include "Widget.h"

void Widget::Remove_Child(const char* Child_Name)
{
	if (Instance)
	{
		if (Has_Child(Child_Name))
		{
			MonoObject* ChildWidget = Mono::Invoke<MonoObject*>(Mono::App_exe, Widget_Class, Instance, "FindWidgetByName", Mono::New_String(Child_Name));
			Mono::Invoke<void>(Mono::App_exe, Widget_Class, ChildWidget, "RemoveFromParent");

			Children.erase(Children.find(Child_Name));
		}
		else
			klog("[Widget] %s(): Child \"%s\" does not exist in Family.\n", __FUNCTION__, Child_Name);
	}
	else
		klog("[Widget] %s(): Instance is not set.\n", __FUNCTION__);
}

void* Widget::Get_Child(const char* Child_Name)
{
	if (Has_Child(Child_Name))
	{
		Children[Child_Name]->Instance = Mono::Invoke<MonoObject*>(Mono::App_exe, Widget_Class, this->Instance, "FindWidgetByName", Mono::New_String(Child_Name));
		return Children[Child_Name];
	}
	else
		klog("[Widget] %s(): Child \"%s\" Does not exist on Parent.\n", __FUNCTION__, Child_Name);
	return 0;
}

bool Widget::Has_Child(const char* Child_Name)
{
	return (Children.find(Child_Name) != Children.end());
}

void Widget::Set_Clip_Children(bool Value)
{
	if (Instance)
		Mono::Set_Property(Widget_Class, Instance, "ClipChildren", Value);
	else
		klog("[Widget] %s(): Instance is not set.\n", __FUNCTION__);
}

Widget::Widget()
{
	this->Widget_Class = Mono::Get_Class(Mono::UI_dll, Mono::PUI_UI2, "Widget");
}

Widget::~Widget()
{

}