#pragma once

namespace UI
{
	class Utilities
	{
	public:
		static char* Get_Version_String();
		static void SetVersionString(const char* str);
		static void ReloadItemList();
		static MonoObject* AppBrowseItem(const char* TitleId, const char* TitleName);
		static MonoObject* Get_Top_Scene();
		static MonoObject* Get_root_Widget();
		static MonoObject* Adjust_Content(int AlignOrientation, float PaddingLeft, float PaddingRight, float PaddingTop, float PaddingBottom);
		static MonoObject* Fit_To_Children();
		static MonoObject* IUFont(int size, int style, int weight);
		static MonoObject* MemoryStream(void* Buffer, int Buffer_Size);
		static void ResetMenuItem(const char* Menu);
		static void RemoveMenuItem(const char* Menu);
		static MonoObject* GetElement(const char* Id);
		static void Set_Value(const char* Id, const char* Value);
		static void AddMenuItem(MonoObject* ElementData);
		static MonoObject* ElementData(const char* Id, const char* Title, const char* Title2, const char* Icon);
		static MonoObject* UIColor(float R, float G, float B, float A);
		static MonoObject* UIColor(float R, float G, float B);
		static float ScreenHeight();
		static float ScreenWidth();
		static bool IsAppRunning(const char* TitleId);
	private:

	};
}