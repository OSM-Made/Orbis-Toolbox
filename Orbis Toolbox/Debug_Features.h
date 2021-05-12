#pragma once

class Debug_Feature
{
public:
	class Custom_Content
	{
	private:
		static Detour* Detour_ExecuteSelectQuery;
		static Detour* Detour_ExecuteCountQuery;
		static Detour* Detour_StartDebugSettings;
		static Detour* Detour_GetIconPath;

		static MonoObject* ExecuteSelectQuery_Hook(MonoObject* Instance, int offset, int limit);
		static int ExecuteCountQuery_Hook(MonoObject* Instance);
		static void StartDebugSettings_Hook(MonoObject* Instance);
		static MonoString* GetIconPath_Hook(MonoObject* item, bool withTheme);

	public:
		static bool Show_App_Home;
		static bool Show_Debug_Settings;

		static void Init();
		static void Term();
	};

	class DebugTitleIdLabel
	{
	private:
		static void(*CreateDebugTitleIdLabel)(MonoObject* Instance);
		static Detour* Detour_ContentDecoratorBase_Constructor;
		static uint64_t ContentDecoratorBase_Constructor_Hook(MonoObject* Instance, uint64_t param);

		static Patcher* Patch_createDevKitPanel;

		static void AddTitleId(MonoObject* m_contentsGridList);
		static void RemoveTitleId(MonoObject* m_contentsGridList);

	public:
		static bool ShowLabels;

		static void Update();
		static void Show();
		static void Hide();
		static void Init();
		static void Term();
	};

	class DevkitPanel
	{
	private:
		static void(*createDevKitPanel)(MonoObject* Instance);
		static Detour* Detour_AreaManager_Constructor;
		static uint64_t AreaManager_Constructor_Hook(MonoObject* Instance);

	public:
		static bool ShowPanel;

		static void Update();
		static void Show();
		static void Hide();
		static bool GetState();
		static void Init();
		static void Term();
	};

private:

};