#pragma once
#include "UINames.h"
#include "DeviceContext.h"

class UIManager
{
public:
	typedef std::vector<UIScreen*> UIList;
	typedef std::unordered_map<std::string, UIScreen*> UITable;

	static void Initialize(HWND hwnd);
	static void Release();
	static void DrawAllUI();

	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;

private:
	UIManager(HWND hwnd);
	~UIManager();
	UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};

	static UIManager* instance;

	UIList uiList;
	UITable uiTable;
};