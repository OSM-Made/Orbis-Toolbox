#pragma once
class Panel;
class Label;

class Widget
{
protected:
	

public:
	MonoObject* Instance;
	std::map<const char*, Widget*> Children;

	template <typename ChildClass>
	void Append_Child(const char* Child_Name, ChildClass* Child_Class)
	{
		if (Instance)
		{
			if (Children.find(Child_Name) == Children.end())
			{
				Children.insert(std::pair<const char*, Widget*>(Child_Name, (Widget*)Child_Class));

				Mono::Invoke<void>(Mono::App_exe, Widget_Class, Instance, "AppendChild", Child_Class->Instance);
			}
			else
				klog("[Widget] %s(): Child \"%s\" already exists in Family.\n", __FUNCTION__, Child_Name);
		}
		else
			klog("[Widget] %s(): Instance is not set.\n", __FUNCTION__);
	}
	void Remove_Child(const char* Child_Name);
	void* Get_Child(const char* Child_Name);
	bool Has_Child(const char* Child_Name);
	void Set_Clip_Children(bool Value);

	Widget();
	~Widget();

private:
	MonoClass* Widget_Class;
};