#include "Common.h"
#include "Build_Overlay.h"

bool Build_Overlay::Draw = false;
Widget* Build_Overlay::Root_Widget = nullptr;

void Build_Overlay::Update()
{
	if (Draw)
	{
		if (Root_Widget->Has_Child("BUILDPANEL"))
			return;

		//Create new Label for the build string.
		Label* BuildLabel = new Label("BUILDLABEL", 20.0f, 36.0f, ORBIS_TOOLBOX_BUILDSTRING, 20, Label::fsItalic,
			Label::fwBold, Label::VerticalAlignment::vCenter, Label::HorizontalAlignment::hCenter, 1.0f, 1.0f, 1.0f, 1.0f);

		//Create new panel for the build Panel.
		Panel* BuildPanel = new Panel("BUILDPANEL", UI::Utilities::ScreenWidth() - (BuildLabel->Get_Text_Width() + 30.0f), 20.0f, 440.0f, 100.0f,
			0.92f, 0.2f, 0.16f, 0.8f, Panel::RenderingOrder::Last, UI::Utilities::Adjust_Content(Panel::Vertical, 4, 4, 4, 4));

		//Append the Text to the Build Panel.
		BuildPanel->Append_Child("BUILDLABEL", BuildLabel);

		//Append the Label to the root widget.
		Root_Widget->Append_Child("BUILDPANEL", BuildPanel);
	}
	else
		Root_Widget->Remove_Child("BUILDPANEL");
}

void Build_Overlay::Init()
{
	//Init the local widget class with our new root widget.
	Root_Widget = new Widget();
	Root_Widget->Instance = UI::Utilities::Get_root_Widget();
}

void Build_Overlay::Term()
{
	//Remove the build panel for destruction.
	Root_Widget->Remove_Child("BUILDPANEL");

	//Clean up alocated classses.
	delete Root_Widget;
}