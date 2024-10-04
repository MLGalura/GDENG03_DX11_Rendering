#include "AppWindow.h"
#include <Windows.h>

struct vec3 {
	float x, y, z;
};

struct vertex {
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};

// DX11 handles vid memory in chunks of 16 bytes, anything more, make sure its aligned or a multiple of 16
__declspec(align(16))
struct constant {
	unsigned int m_time;
};

AppWindow::AppWindow() : spawntimer(0.0f)
{
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
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top); 

	// CHECK - hard coded for delta time, find function to fix
	this->spawntimer += 0.016f;

	if (this->spawntimer >= .15f) {
		this->spawntimer = 0.0f;

		// Spawns in bursts
		for (int i = 0; i < 1; i++) {
			// Random X and Y positions between -1.0f and 1.0f
			float randXpos = (rand() / (float)RAND_MAX) * 0.3f - 0.15f;
			float randYpos = (rand() / (float)RAND_MAX) * -0.25f - 0.15f;

			// Random X velocity - only -2.0f or 2.0f
			//float randXVel = (rand() % 2 == 0) ? -2.0f : 2.0f;
			float randXVel = (rand() / (float)RAND_MAX) * 2.0f - 2.0f;
			float randYVel = (rand() / (float)RAND_MAX) * 0.3f - 0.15f;

			// Random lifetime between 3.0f and 8.0f
			float randLT = 3.0f + ((rand() / (float)RAND_MAX) * 5.0f);

			this->m_particleList.push_back(new Particle(randXpos, randYpos, randXVel, 0.25f, randLT));
		}
	}

	for (auto& particle : this->m_particleList) {
		particle->update();
	}

	this->m_swap_chain->present(true); 
}

void AppWindow::onDestroy()
{

	for (auto& particle : this->m_particleList) {
		particle->release();
	}
	m_particleList.clear();

	Window::onDestroy();
	this->m_vb->release();
	this->m_swap_chain->release();
	this->m_vs->release();
	this->m_ps->release();
	GraphicsEngine::get()->release();
}
