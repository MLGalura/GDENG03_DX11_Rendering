#include "HierarchyScreen.h"

HierarchyScreen::HierarchyScreen() : UIScreen("HIERARCHY_SCREEN")
{

}

HierarchyScreen::~HierarchyScreen()
{

}

void HierarchyScreen::DrawUI()
{
	static float color[4] = { 1.0f, 0.5f, 0.3f, 1.0f };

	ImGui::Begin("Credits");
	ImGui::ColorPicker4("Color", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueWheel);
	ImGui::End();
}
