#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

struct vec2 {
	float x, y;
};

struct vertex {
	Vector3D position;
	Vector3D color;
	Vector3D color1;
	vec2 uv;
};

// DX11 handles vid memory in chunks of 16 bytes, anything more, make sure its aligned or a multiple of 16
__declspec(align(16))
struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;

	unsigned int m_time;
};

Vector3D m_velocity = Vector3D(0.5f, 0.5f, 0.0f); 

AppWindow::AppWindow()
{
}

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	// Update position based on velocity and delta time
	m_pos.m_x += m_velocity.m_x * m_delta_time * m_speed;
	m_pos.m_y += m_velocity.m_y * m_delta_time * m_speed;

	// Apply translation based on updated position
	Matrix4x4 temp;
	temp.setTranslation(m_pos);

	// Set scale and multiply by the translation matrix
	cc.m_world.setScale(Vector3D(0.5f, 0.5f, 0.5f));
	cc.m_world *= temp;

	// Apply view and projection matrices
	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f, 4.0f
	);

	// Update the constant buffer with the new world matrix
	this->m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	// Check for collisions with the window borders and bounce
	this->checkBorders();
}

void AppWindow::checkBorders()
{
	RECT rc = this->getClientWindowRect();
	float windowWidth = (rc.right - rc.left) / 400.0f; 
	float windowHeight = (rc.bottom - rc.top) / 400.0f; 

	float halfWidth = 0.15f;  
	float halfHeight = 0.15f; 

	// Check if hitting the left or right borders
	if (m_pos.m_x - halfWidth <= -windowWidth / 2.0f || m_pos.m_x + halfWidth >= windowWidth / 2.0f)
		m_velocity.m_x = -m_velocity.m_x; // Reverse the X direction

	// Check if hitting the top or bottom borders
	if (m_pos.m_y - halfHeight <= -windowHeight / 2.0f || m_pos.m_y + halfHeight >= windowHeight / 2.0f)
		m_velocity.m_y = -m_velocity.m_y; // Reverse the Y direction
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	this->m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	this->m_swap_chain->init(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();

	float width = rc.right - rc.left;
	float height = rc.bottom - rc.top;

	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(width, height);

	// RUDIMENTARY INPUT SYSTEM IMPLEMENTATION
	short spaceState = ::GetAsyncKeyState(VK_SPACE);
	short backState = ::GetAsyncKeyState(VK_BACK);
	short delstate = ::GetAsyncKeyState(VK_DELETE);
	short escstate = ::GetAsyncKeyState(VK_ESCAPE);

	// SPAWNING
	if ((spaceState & 0x8000) && !SPACE) {
		float velocities[] = {-0.25f, 0.25f, -0.15f, 0.15f, 0.0f};
		float randX;
		float randY;

		randX = velocities[std::rand() % 5];

		if (randX != 0.0f)
			randY = velocities[std::rand() % 5];

		else
			randY = velocities[std::rand() % 4];


		this->m_circleList.push_back(new Circle());
		this->m_circleList[this->m_circleList.size()-1]->init(randX, randY); 

		SPACE = true;
		std::cout << "[SPACEBAR] was pressed. Spawning new Circle." << std::endl;
	}

	else if (!(spaceState & 0x8000) && SPACE)
		SPACE = false;

	// POPPING
	if ((backState & 0x8000) && !BACK)
	{
		if (!m_circleList.empty()) {
			m_circleList.pop_back(); 
			std::cout << "[BACKSPACE] was pressed. Popping the Circle vector." << std::endl;
		}

		else
			std::cout << "[BACKSPACE] was pressed, but no Circles in vector." << std::endl;

		BACK = true;  
	}

	else if (!(backState & 0x8000) && BACK)
		BACK = false; 

	// DELETING
	if ((delstate & 0x8000) && !DEL)
	{
		this->m_circleList.clear();
		DEL = true;
		std::cout << "[DELETE] was pressed. Clearing the Circle vector." << std::endl;
	}

	else if (!(delstate & 0x8000) && DEL)
		DEL = false;


	if (this->m_circleList.size() > 0) 
	{
		for (auto& circle : this->m_circleList) {
			circle->update(m_delta_time, width, height);
			circle->draw();
		}
	}

	this->m_swap_chain->present(true);

	// EXITING
	if (escstate & 0x8000) {
		this->onDestroy();
		std::cout << "[ESCAPE] was pressed. Terminating the program." << std::endl;
	}

	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = ::GetTickCount();

	this->m_delta_time = (this->m_old_delta) ? (this->m_new_delta - this->m_old_delta) / 1000.0f : 0;

	//this->updateQuadPosition();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	//this->m_vb->release();
	this->m_swap_chain->release();
	/*this->m_vs->release();
	this->m_ps->release();*/
	GraphicsEngine::get()->release();
}
