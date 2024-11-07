#include "UIScreen.h"

class ProfilerScreen : public UIScreen
{
protected:
	ProfilerScreen();
	~ProfilerScreen();
	virtual void DrawUI() override;

	friend class UIManager;
};
