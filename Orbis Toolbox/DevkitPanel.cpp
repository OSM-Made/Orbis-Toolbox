#include "Common.h"
#include "UI.h"

void(*UI::DevkitPanel::createDevKitPanel)(MonoObject* Instance) = nullptr;
Detour* UI::DevkitPanel::Detour_AreaManager_Constructor = nullptr;

bool UI::DevkitPanel::ShowPanel = false;

uint64_t UI::DevkitPanel::AreaManager_Constructor_Hook(MonoObject* Instance)
{
	uint64_t res = Detour_AreaManager_Constructor->Stub<uint64_t>(Instance);

	if (ShowPanel)
		createDevKitPanel(Instance);

	return res;
}

void UI::DevkitPanel::Show()
{
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	MonoObject* AreaManager_Instance = Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance");
	MonoObject* m_devKitPanel = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_devKitPanel");
	MonoClass* Widget = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "Widget");

	// AreaManager.Instance.m_devKitPanel
	// If m_devKitPanel is null we must create the panel first.
	if (!m_devKitPanel)
	{
		createDevKitPanel(Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance"));
	}
	else
	{
		MonoClass* UITimer = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "UITimer");
		MonoObject* m_updatePanelTimer = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_updatePanelTimer");

		// AreaManager.Instance.m_updatePanelTimer.Start()
		// If the m_updatePanelTimer is initialized start the timer.
		if (m_updatePanelTimer)
			Mono::Invoke<void>(Mono::Highlevel_UI2, UITimer, m_updatePanelTimer, "Start");

		// AreaManager.Instance.m_devKitPanel.Show()
		// Show the panel.
		MonoClass* UINode = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "UINode");
		Mono::Invoke<void>(Mono::Highlevel_UI2, UINode, m_devKitPanel, "Show");
	}

	ShowPanel = true;
}

void UI::DevkitPanel::Hide()
{
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	MonoObject* AreaManager_Instance = Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance");

	MonoClass* UITimer = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "UITimer");
	MonoObject* m_updatePanelTimer = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_updatePanelTimer");

	// AreaManager.Instance.m_updatePanelTimer.Stop()
	// If the m_updatePanelTimer is initialized stop the timer.
	if (m_updatePanelTimer)
		Mono::Invoke<void>(Mono::Highlevel_UI2, UITimer, m_updatePanelTimer, "Stop");

	// AreaManager.Instance.m_devKitPanel.Hide()
	// Hide the panel.
	MonoObject* m_devKitPanel = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_devKitPanel");
	if (m_devKitPanel)
	{
		MonoClass* UINode = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "UINode");
		Mono::Invoke<void>(Mono::Highlevel_UI2, UINode, m_devKitPanel, "Hide");
	}

	ShowPanel = false;
}

bool UI::DevkitPanel::GetState()
{
	MonoClass* FrameTask = Mono::Get_Class(Mono::Highlevel_UI2, "Sce.PlayStation.HighLevel.UI2", "FrameTask");
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	MonoObject* AreaManager_Instance = Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance");
	MonoObject* m_devKitPanel = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_devKitPanel");
	MonoObject* m_updatePanelTimer = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_updatePanelTimer");

	if (m_devKitPanel && m_updatePanelTimer)
		return !Mono::Get_Property<bool>(FrameTask, m_updatePanelTimer, "IsStopped");
	else
		return false;
}

void UI::DevkitPanel::Init()
{
	//Get Method to make devkit panel
	uint64_t createDevKitPanel_addr = Mono::Get_Address_of_Method(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "createDevKitPanel", 0);
	createDevKitPanel = decltype(createDevKitPanel)(createDevKitPanel_addr);

	//Hook AreaManager Constructor
	Detour_AreaManager_Constructor = new Detour();
	Detour_AreaManager_Constructor->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", ".ctor", 0, (void*)AreaManager_Constructor_Hook, 15);
}

void UI::DevkitPanel::Term()
{
	delete Detour_AreaManager_Constructor;
}
