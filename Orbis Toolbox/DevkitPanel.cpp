#include "Common.h"
#include "Debug_Features.h"

void(*Debug_Feature::DevkitPanel::createDevKitPanel)(MonoObject* Instance) = nullptr;
Detour* Debug_Feature::DevkitPanel::Detour_AreaManager_Constructor = nullptr;

bool Debug_Feature::DevkitPanel::ShowPanel = false;

uint64_t Debug_Feature::DevkitPanel::AreaManager_Constructor_Hook(MonoObject* Instance)
{
	uint64_t res = Detour_AreaManager_Constructor->Stub<uint64_t>(Instance);

	if (ShowPanel)
		createDevKitPanel(Instance);

	return res;
}

void Debug_Feature::DevkitPanel::Update()
{
	Debug_Feature::DevkitPanel::ShowPanel ? Show() : Hide();
}

void Debug_Feature::DevkitPanel::Show()
{
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	MonoObject* AreaManager_Instance = Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance");
	MonoObject* m_devKitPanel = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_devKitPanel");
	MonoClass* Widget = Mono::Get_Class(Mono::UI_dll, Mono::PUI_UI2, "Widget");

	// AreaManager.Instance.m_devKitPanel
	// If m_devKitPanel is null we must create the panel first.
	if (!m_devKitPanel)
	{
		createDevKitPanel(Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance"));
	}
	else
	{
		MonoClass* UITimer = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UITimer");
		MonoObject* m_updatePanelTimer = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_updatePanelTimer");

		// AreaManager.Instance.m_updatePanelTimer.Start()
		// If the m_updatePanelTimer is initialized start the timer.
		if (m_updatePanelTimer)
			Mono::Invoke<void>(Mono::UI_dll, UITimer, m_updatePanelTimer, "Start");

		// AreaManager.Instance.m_devKitPanel.Show()
		// Show the panel.
		MonoClass* UINode = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UINode");
		Mono::Invoke<void>(Mono::UI_dll, UINode, m_devKitPanel, "Show");
	}

	ShowPanel = true;
}

void Debug_Feature::DevkitPanel::Hide()
{
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	MonoObject* AreaManager_Instance = Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance");

	MonoClass* UITimer = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UITimer");
	MonoObject* m_updatePanelTimer = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_updatePanelTimer");

	// AreaManager.Instance.m_updatePanelTimer.Stop()
	// If the m_updatePanelTimer is initialized stop the timer.
	if (m_updatePanelTimer)
		Mono::Invoke<void>(Mono::UI_dll, UITimer, m_updatePanelTimer, "Stop");

	// AreaManager.Instance.m_devKitPanel.Hide()
	// Hide the panel.
	MonoObject* m_devKitPanel = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_devKitPanel");
	if (m_devKitPanel)
	{
		MonoClass* UINode = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "UINode");
		Mono::Invoke<void>(Mono::UI_dll, UINode, m_devKitPanel, "Hide");
	}

	ShowPanel = false;
}

bool Debug_Feature::DevkitPanel::GetState()
{
	MonoClass* FrameTask = Mono::Get_Class(Mono::UI_dll, Mono::PUI, "FrameTask");
	MonoClass* AreaManager = Mono::Get_Class(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager");
	MonoObject* AreaManager_Instance = Mono::Get_Instance(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "Instance");
	MonoObject* m_devKitPanel = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_devKitPanel");
	MonoObject* m_updatePanelTimer = Mono::Get_Field<MonoObject*>(AreaManager, AreaManager_Instance, "m_updatePanelTimer");

	if (m_devKitPanel && m_updatePanelTimer)
		return !Mono::Get_Property<bool>(FrameTask, m_updatePanelTimer, "IsStopped");
	else
		return false;
}

void Debug_Feature::DevkitPanel::Init()
{
	//Get Method to make devkit panel
	uint64_t createDevKitPanel_addr = Mono::Get_Address_of_Method(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", "createDevKitPanel", 0);
	createDevKitPanel = decltype(createDevKitPanel)(createDevKitPanel_addr);

	//Hook AreaManager Constructor
	Detour_AreaManager_Constructor = new Detour();
	Detour_AreaManager_Constructor->DetourMethod(Mono::App_exe, "Sce.Vsh.ShellUI.TopMenu", "AreaManager", ".ctor", 0, (void*)AreaManager_Constructor_Hook);
}

void Debug_Feature::DevkitPanel::Term()
{
	delete Detour_AreaManager_Constructor;
}
