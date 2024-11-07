#pragma once
#include "UIScreen.h"

class HierarchyScreen : public UIScreen
{
protected:
	HierarchyScreen();
	~HierarchyScreen();
	virtual void DrawUI() override;

	friend class UIManager;
};

