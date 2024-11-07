#include "MenuScreen.h"

MenuScreen::MenuScreen() : UIScreen("MENU_SCREEN")
{

}

MenuScreen::~MenuScreen()
{

}

void MenuScreen::DrawUI()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			this->ShowMenuFile();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Game Object"))
		{
			ImGui::EndMenu();
		}
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::SameLine();
		ImGui::PopStyleColor();

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();
}

void MenuScreen::ShowMenuFile()
{
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Save")) {}
	if (ImGui::MenuItem("Save As")) {}
	if (ImGui::MenuItem("Load")) {}
}