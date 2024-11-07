#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Window.h"

#include <string>

class UIScreen
{
protected:
	UIScreen(std::string name);
	~UIScreen();

	std::string GetName();
	virtual void DrawUI() = 0;

	std::string name;

	friend class UIManager;
};

