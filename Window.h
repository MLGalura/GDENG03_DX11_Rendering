#pragma once

#include <Windows.h>

class Window {

public:
	// Constructor & Destructor
	Window();
	~Window();

	// Determines if the window is currently running
	bool isRunning();

	// Retrieve the internal size of the window
	RECT getClientWindowRect();

	// Events
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();

	virtual void onFocus() = 0;
	virtual void onKillFocus() = 0;

private:
	// Dispatches the procedure to be called
	bool broadcast();

protected:
	HWND m_HWND;
	bool m_isRunning;
	bool m_is_init = false;
};

