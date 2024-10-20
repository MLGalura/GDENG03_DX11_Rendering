#pragma once

#include <Windows.h>

class Window {

public:
	// Constructor & Destructor
	Window();
	~Window();

	// Initializes and releases the window
	bool init();
	bool release();

	// Dispatches the procedure to be called
	bool broadcast();

	// Determines if the window is currently running
	bool isRunning();

	// Retrieve the internal size of the window
	RECT getClientWindowRect();

	void setHWND(HWND hwnd);

	// Events
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();

	virtual void onFocus() = 0;
	virtual void onKillFocus() = 0;

protected:
	HWND m_HWND;
	bool m_isRunning;
};

