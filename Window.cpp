#include "Window.h"
#include "imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global Variables
Window* window = nullptr;

Window::Window()
{
	// Initialize the variables in the constructor
	this->m_HWND = NULL;
	this->m_isRunning = NULL;
}

Window::~Window()
{

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	switch (msg) 
	{
		// Event 
		case WM_CREATE:
			//window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			//SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);

			window->setHWND(hwnd);
			window->onCreate();
			break;

		// Event fired when window gets focused
		case WM_SETFOCUS:
			//Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onFocus();
			break;
			
		// Event fired when window gets focused
		case WM_KILLFOCUS:
			//Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onKillFocus();
			break;

		// Event where window made a request to terminate
		case WM_DESTROY:
			//Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onDestroy();
			::PostQuitMessage(0);
			break;


		// If the message is neither create nor destroy, set default window
		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::init()
{
	// Define the class of the parameter for RegisterClassEx()
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = WndProc; // Procedure to handle events for the window

	// Defines some properties regarding style and visuals of the windows
	if (!::RegisterClassEx(&wc))
		return false;

	if (!window)
		window = this;

	// Creates the window
	this->m_HWND = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
					 NULL, NULL, NULL, this);

	// Returns false if the creation results in failure
	if (!this->m_HWND)
		return false;

	// Draws the window
	::ShowWindow(this->m_HWND, SW_SHOW);
	::UpdateWindow(this->m_HWND);

	// Set this flag to true to indicate that the window is initialized and running
	this->m_isRunning = true;

	return true;
}

bool Window::release()
{
	// Destroys the window
	if (::DestroyWindow(this->m_HWND))
		return false;

	return true;
}

bool Window::broadcast()
{
	MSG msg;

	// Update can be placed here as broadcast() will be called in the main loop
	window->onUpdate();

	// Retrieves from the queue of messages of the OS until emptied
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&msg); // Message to be translated for the OS
		DispatchMessageW(&msg); // To be processed by the window procedure
	}

	// Prevents the CPU from throttling
	Sleep(1);

	return true;
}

bool Window::isRunning()
{
	return this->m_isRunning;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_HWND, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_HWND = hwnd;
}

void Window::onDestroy()
{
	this->m_isRunning = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}
