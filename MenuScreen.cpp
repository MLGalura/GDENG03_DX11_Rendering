#include "MenuScreen.h"
#include <DirectXTex.h>
#include "GraphicsEngine.h"
#include "RenderSystem.h"

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
		if (ImGui::BeginMenu("About"))
		{
			this->ShowMenuFile();
			ImGui::EndMenu();
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::SameLine();
		ImGui::PopStyleColor();

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();

	if (this->showCredits) {
		const wchar_t* path = L"Sparkle.png";
		ID3D11Resource* tex = nullptr;
		ID3D11ShaderResourceView* texView = nullptr;

		DirectX::ScratchImage img_data;
		HRESULT hr = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_NONE, nullptr, img_data);
		if (FAILED(hr) || img_data.GetImageCount() == 0) {
			throw std::exception("Failed to load image or image data is empty.");
		}

		hr = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->getDevice(),
			img_data.GetImages(), img_data.GetImageCount(),
			img_data.GetMetadata(), &tex);
		if (FAILED(hr) || tex == nullptr) {
			throw std::exception("Texture not created successfully.");
		}

		hr = GraphicsEngine::get()->getRenderSystem()->getDevice()->CreateShaderResourceView(tex, nullptr, &texView);
		if (FAILED(hr) || texView == nullptr) {
			tex->Release();
			throw std::exception("Failed to create shader resource view.");
		}

		ImGui::Begin("Credits", &this->showCredits, ImGuiWindowFlags_NoCollapse);
		if (texView) {
			ImGui::Image((ImTextureID)texView, ImVec2(256, 256));
			ImGui::Text("About");
			ImGui::Text("");
			ImGui::Text("GDENG03 DX11 Scene Editor");
			ImGui::Text("Developer: Marc Lawrence C. Galura");
			ImGui::Text("Class: GDENG03 - XX22");
			ImGui::Text("");
			ImGui::Text("Acknowledgements:");
			ImGui::Text("PardCode Game Engines Tutorial");
			ImGui::Text("Dr. Neil Patrick Del Gallego");
		}
		ImGui::End();
	}

	if (this->showColor) {
		static float color[4] = { 1.0f, 0.5f, 0.3f, 1.0f };

		ImGui::Begin("Color Picker", &this->showColor, ImGuiWindowFlags_NoCollapse);
		ImGui::ColorPicker4("Color", color, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::End();
	}
}

void MenuScreen::ShowMenuFile()
{
	if (ImGui::MenuItem("Credits")) 
	{
		this->showCredits = true;
	}

	if (ImGui::MenuItem("Colors")) {
		this->showColor = true;
	}
}