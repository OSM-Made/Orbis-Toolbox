#pragma once

class Mono
{
private:
	static MonoDomain* Root_Domain;

public:
	static MonoImage* App_exe;
	static MonoImage* PlayStation_Core;
	static MonoImage* platform_dll;
	static MonoImage* UI_dll;
	static MonoImage* KernelSysWrapper;
	static MonoImage* mscorlib;
	static MonoImage* Accessor_Db;
	static MonoImage* Vsh_Lx;
	static MonoImage* SysfileUtilWrapper;

	static int Software_Version;
	static const char* PUI;
	static const char* PUI_UI2;
	static const char* PUI_UI3;

	static bool Init();
	static void MonoLog(const char* fmt, ...);
	static MonoImage* Get_Image(const char* MonoAssembly, ...);

	//
	// Classes
	//
	static MonoClass* Get_Class(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name);

	//
	// Objects
	//
	static MonoObject* New_Object(MonoClass* Klass);
	static MonoObject* New_Object(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name);
	static MonoString* New_String(const char* str, ...);
	static MonoArray* New_Array(MonoClass* eclass, int size);

	//
	// Methods
	//
	static uint64_t Get_Address_of_Method(MonoImage* Assembly_Image, const char* Name_Space, const char* Class_Name, const char* Method_Name, int Param_Count);
	static uint64_t Get_Address_of_Method(MonoImage* Assembly_Image, MonoClass* klass, const char* Method_Name, int Param_Count);

	template <typename result, typename... Args>
	static result Invoke(MonoImage* Assembly_Image, MonoClass* klass, MonoObject* Instance, const char* Method_Name, Args... args)
	{
		void* Argsv[] = { &args... };
		uint64_t ThunkAddress = Get_Address_of_Method(Assembly_Image, klass, Method_Name, ARRAY_COUNT(Argsv));

		if (!ThunkAddress)
		{
			MonoLog("Invoke: Failed to get thunk address for \"%s\".", Method_Name);
			return (result)NULL;
		}

		if (Instance)
		{
			result(*Method)(MonoObject* Instance, Args... args) = decltype(Method)(ThunkAddress);
			return Method(Instance, args...);
		}
		else //Static Call.
		{
			result(*Method)(Args... args) = decltype(Method)(ThunkAddress);
			return Method(args...);
		}
	}

	//
	// Properties
	//
	static MonoObject* Get_Instance(MonoClass* Klass, const char* Instance);
	static MonoObject* Get_Instance(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name, const char* Instance);

	template <typename result>
	static result Get_Property(MonoClass* Klass, MonoObject* Instance, const char* Property_Name)
	{
		if (Klass == nullptr)
		{
			MonoLog("Get_Property: Klass was null.");
			return (result)0;
		}

		/*if (Instance == nullptr)
		{
			MonoLog("Get_Property: Instance was null.");
			return (result)0;
		}*/

		MonoProperty* Prop = mono_class_get_property_from_name(Klass, Property_Name);

		if (Prop == nullptr)
		{
			MonoLog("Get_Property: Property \"%s\" could not be found on class \"%s\".", Property_Name, Klass->name);
			return (result)NULL;
		}

		MonoMethod* Get_Method = mono_property_get_get_method(Prop);

		if (Get_Method == nullptr)
		{
			MonoLog("Get_Property: Could not find Get Method for \"%s\" in class \"%s\".", Property_Name, Klass->name);
			return (result)NULL;
		}

		uint64_t Get_Method_Thunk = (uint64_t)mono_aot_get_method(Root_Domain, Get_Method);

		if (Get_Method_Thunk == NULL)
		{
			MonoLog("Get_Property: Could not get thunk address for \"%s\" in class \"%s\".", Property_Name, Klass->name);
			return (result)NULL;
		}

		if (Instance != nullptr)
		{
			result(*Method)(MonoObject* Instance) = decltype(Method)(Get_Method_Thunk);
			return Method(Instance);
		}
		else
		{
			result(*Method)() = decltype(Method)(Get_Method_Thunk);
			return Method();
		}
	}

	template <typename result>
	static result Get_Property_Invoke(MonoClass* Klass, MonoObject* Instance, const char* Property_Name)
	{
		if (Klass == nullptr)
		{
			MonoLog("Set_Property: Klass was null.");
			return (result)0;
		}

		if (Instance == nullptr)
		{
			MonoLog("Set_Property: Instance was null.");
			return (result)0;
		}

		MonoProperty* Prop = mono_class_get_property_from_name(Klass, Property_Name);

		if (Prop == nullptr)
		{
			MonoLog("Get_Property: Property \"%s\" could not be found on class \"%s\".", Property_Name, Klass->name);
			return (result)NULL;
		}

		MonoMethod* Get_Method = mono_property_get_get_method(Prop);

		if (Get_Method == nullptr)
		{
			MonoLog("Get_Property: Could not find Get Method for \"%s\" in class \"%s\".", Property_Name, Klass->name);
			return (result)NULL;
		}

		uint64_t obj = (uint64_t)mono_runtime_invoke(Get_Method, Instance, nullptr, NULL);

		if (std::is_pointer<result>::value)
			return (result)obj;
		else
			return *(result*)mono_object_unbox((MonoObject*)obj);
	}

	template <typename result>
	static result Get_Property(MonoObject* Instance, const char* Property_Name)
	{
		if (Instance == nullptr)
		{
			MonoLog("Set_Property: Instance was null.");
			return (result)0;
		}

		if (Instance->vtable == nullptr)
		{
			MonoLog("Set_Property: Instance->vtable was null.");
			return (result)0;
		}

		if (Instance->vtable->klass == nullptr)
		{
			MonoLog("Set_Property: Instance->vtable->klass was null.");
			return (result)0;
		}

		return Get_Property<result>(Instance->vtable->klass, Instance, Property_Name);
	}

	template <typename result>
	static result Get_Property(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name, MonoObject* Instance, const char* Property_Name)
	{
		return Get_Property<result>(Mono::Get_Class(Assembly_Image, Namespace, Class_Name), Instance, Property_Name);
	}

	template <typename Param>
	static void Set_Property(MonoClass* Klass, MonoObject* Instance, const char* Property_Name, Param Value)
	{
		if (Klass == nullptr)
		{
			MonoLog("Set_Property: Klass was null.");
			return;
		}

		if (Instance == nullptr)
		{
			MonoLog("Set_Property: Instance was null.");
			return;
		}

		MonoProperty* Prop = mono_class_get_property_from_name(Klass, Property_Name);

		if (Prop == nullptr)
		{
			MonoLog("Set_Property: Property \"%s\" could not be found on class \"%s\".", Property_Name, Klass->name);
			return;
		}

		MonoMethod* Set_Method = mono_property_get_set_method(Prop);

		if (Set_Method == nullptr)
		{
			MonoLog("Set_Property: Could not find Set Method for \"%s\" in class \"%s\".", Property_Name, Klass->name);
			return;
		}

		uint64_t Set_Method_Thunk = (uint64_t)mono_aot_get_method(Root_Domain, Set_Method);

		if (Set_Method_Thunk == NULL)
		{
			MonoLog("Set_Property: Could not get thunk address for \"%s\" in class \"%s\".", Property_Name, Klass->name);
			return;
		}

		void(*Method)(MonoObject* Instance, Param Value) = decltype(Method)(Set_Method_Thunk);
		Method(Instance, Value);
	}

	template <typename Param>
	static void Set_Property_Invoke(MonoClass* Klass, MonoObject* Instance, const char* Property_Name, Param Value)
	{
		if (Klass == nullptr)
		{
			MonoLog("Set_Property: Klass was null.");
			return;
		}

		if (Instance == nullptr)
		{
			MonoLog("Set_Property: Instance was null.");
			return;
		}

		MonoProperty* Prop = mono_class_get_property_from_name(Klass, Property_Name);

		if (Prop == nullptr)
		{
			MonoLog("Set_Property: Property \"%s\" could not be found on class \"%s\".", Property_Name, Klass->name);
			return;
		}

		MonoMethod* Set_Method = mono_property_get_set_method(Prop);

		if (Set_Method == nullptr)
		{
			MonoLog("Set_Property: Could not find Set Method for \"%s\" in class \"%s\".", Property_Name, Klass->name);
			return;
		}

		mono_runtime_invoke(Set_Method, Instance, (void**)&Value, NULL);
	}

	template <typename Param>
	static void Set_Property_test(MonoClass* Klass, MonoObject* Instance, const char* Property_Name, Param Value)
	{
		if (Klass == nullptr)
		{
			MonoLog("Set_Property: Klass was null.");
			return;
		}

		if (Instance == nullptr)
		{
			MonoLog("Set_Property: Instance was null.");
			return;
		}

		MonoProperty* Prop = mono_class_get_property_from_name(Klass, Property_Name);

		if (Prop == nullptr)
		{
			MonoLog("Set_Property: Property \"%s\" could not be found on class \"%s\".", Property_Name, Klass->name);
			return;
		}

		MonoMethod* Set_Method = mono_property_get_set_method(Prop);

		if (Set_Method == nullptr)
		{
			MonoLog("Set_Property: Could not find Set Method for \"%s\" in class \"%s\".", Property_Name, Klass->name);
			return;
		}
		void* Argsv[] = { &Value };
		mono_runtime_invoke(Set_Method, Instance, Argsv, NULL);
	}
	
	template <typename Param>
	static void Set_Property(MonoObject* Instance, const char* Property_Name, Param Value)
	{
		
		if (Instance == nullptr)
		{
			MonoLog("Set_Property: Instance was null.");
			return;
		}

		if (Instance->vtable == nullptr)
		{
			MonoLog("Set_Property: Instance->vtable was null.");
			return;
		}

		if (Instance->vtable->klass == nullptr)
		{
			MonoLog("Set_Property: Instance->vtable->klass was null.");
			return;
		}

		MonoClass* Klass = Instance->vtable->klass;

		Set_Property<Param>(Instance->vtable->klass, Instance, Property_Name, Value);
	}

	template <typename Param>
	static void Set_Property(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name, MonoObject* Instance, const char* Property_Name, Param Value)
	{
		Set_Property<Param>(Mono::Get_Class(Assembly_Image, Namespace, Class_Name), Instance, Property_Name, Value);
	}

	//
	// Fields
	//
	template <typename result>
	static result Get_Field(MonoClass* Klass, MonoObject* Instance, const char* Field_Name)
	{
		if (Klass == nullptr)
		{
			MonoLog("Get_Field: Klass was null.");
			return (result)0;
		}

		/*if (Instance == nullptr)
		{
			MonoLog("Get_Field: Instance was null.");
			return (result)0;
		}*/

		MonoClassField* Field = mono_class_get_field_from_name(Klass, Field_Name);

		if (Field == nullptr)
		{
			MonoLog("Get_Field: Failed to find get Field \"%s\" in Class \"%s\".", Field_Name, Klass->name);
			return (result)0;
		}

		result Value;
		mono_field_get_value(Instance, Field, &Value);

		return Value;
	}

	template <typename result>
	static result Get_Field(MonoObject* Instance, const char* Field_Name)
	{
		if (Instance == nullptr)
		{
			MonoLog("Get_Field: Instance was null.");
			return (result)0;
		}

		if (Instance->vtable == nullptr)
		{
			MonoLog("Get_Field: Instance->vtable was null.");
			return (result)0;
		}

		if (Instance->vtable->klass == nullptr)
		{
			MonoLog("Get_Field: Instance->vtable->klass was null.");
			return (result)0;
		}

		return Get_Field<result>(Instance->vtable->klass, Instance, Field_Name);
	}

	template <typename result>
	static result Get_Field(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name, MonoObject* Instance, const char* Field_Name)
	{
		return Get_Field<result>(Mono::Get_Class(Assembly_Image, Namespace, Class_Name), Instance, Field_Name);
	}

	template <typename Param>
	static void Set_Field(MonoClass* Klass, MonoObject* Instance, const char* Field_Name, Param Value)
	{
		if (Klass == nullptr)
		{
			MonoLog("Set_Field: Klass was null.");
			return;
		}

		if (Instance == nullptr)
		{
			MonoLog("Set_Field: Instance was null.");
			return;
		}

		MonoClassField* Field = mono_class_get_field_from_name(Klass, Field_Name);

		if (Field == nullptr)
		{
			MonoLog("Set_Field: Failed to find get Field \"%s\" in Class \"%s\".", Field_Name, Klass->name);
			return;
		}

		mono_field_set_value(Instance, Field, &Value);
	}

	template <typename Param>
	static void Set_Field(MonoObject* Instance, const char* Field_Name, Param Value)
	{
		if (Instance == nullptr)
		{
			MonoLog("Set_Field: Instance was null.");
			return;
		}

		if (Instance->vtable == nullptr)
		{
			MonoLog("Set_Field: Instance->vtable was null.");
			return;
		}

		if (Instance->vtable->klass == nullptr)
		{
			MonoLog("Set_Field: Instance->vtable->klass was null.");
			return;
		}

		Set_Field<Param>(Instance->vtable->klass, Instance, Field_Name, Value);
	}

	template <typename Param>
	static void Set_Field(MonoImage* Assembly_Image, const char* Namespace, const char* Class_Name, MonoObject* Instance, const char* Field_Name, Param Value)
	{
		Set_Field<Param>(Mono::Get_Class(Assembly_Image, Namespace, Class_Name), Instance, Field_Name, Value);
	}
};