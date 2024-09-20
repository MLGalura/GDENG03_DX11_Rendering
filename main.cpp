#include "AppWindow.h"

int main() 
{
	// Initialize the AppWindow
	AppWindow app;
	
	if (app.init()) {

		while (app.isRunning()) {
			app.broadcast();
		}  
	}


	return 0;
} 