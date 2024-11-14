#include "AppWindow.h"
#include <Windows.h>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

#include "InputSystem.h"
#include "UIManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	cc.time = ::GetTickCount();

	this->m_delta_pos += this->m_delta_time / 10.0f;

	if (this->m_delta_pos > 1.0f)
		this->m_delta_pos = 0.0f;

	Matrix4x4 temp;

	this->m_delta_scale += this->m_delta_time / 1.0f;

	cc.world.setIdentity();


	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(this->m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(this->m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = this->m_world_cam.getTranslation() + world_cam.getZDirection() * (this->m_forward * 0.3f);

	new_pos = new_pos + world_cam.getXDirection() * (this->m_rightward * 0.3f);

	new_pos = new_pos + world_cam.getYDirection() * (this->m_upward * 0.3f);

	world_cam.setTranslation(new_pos);

	this->m_world_cam = world_cam;

	world_cam.inverse();

	cc.view = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.proj.setPerspectiveFovLH(1.5f, ((float)width / (float)height), 0.1f, 100.0f);

	this->m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(true);

	this->m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\brick.jpg");
	//this->m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\teapot.obj");

	RECT rc = this->getClientWindowRect();

	this->m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);
	this->m_world_cam.setTranslation(Vector3D(0.0f, 0.0f, -2.0f));


	this->obj = new Object("Teapot", nullptr, NULL); 
	this->obj->SetTexture(L"Assets\\Textures\\brick.png"); 
	this->obj->SetMesh(L"Assets\\Meshes\\teapot.obj");
	

	cc.time = 0;
	this->m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	UIManager::Initialize(this->m_HWND);


	
}

void AppWindow::onUpdate()
{
	InputSystem::get()->update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	// GraphicsEngine::get()->setShaders();
	
	float width = rc.right - rc.left;
	float height = rc.bottom - rc.top;

	this->update();

	this->obj->Update(this->m_delta_time);
	this->obj->Draw(this->cc.view, this->cc.proj);

	UIManager::DrawAllUI();

	this->m_swap_chain->present(true);

	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = ::GetTickCount();

	this->m_delta_time = (this->m_old_delta) ? (this->m_new_delta - this->m_old_delta) / 1000.0f : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
		this->m_forward = 1.0f;

	else if (key == 'S')
		this->m_forward = -1.0f;

	else if (key == 'A')
		this->m_rightward = -1.0f;

	else if (key == 'D')
		this->m_rightward = 1.0f;

	else if (key == 'Q')
		this->m_upward = -1.0f;

	else if (key == 'E')
		this->m_upward = 1.0f;

	else if (key == 27)
		exit(0);
}

void AppWindow::onKeyUp(int key)
{
	this->m_forward = 0.0f;
	this->m_rightward = 0.0f;
	this->m_upward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	this->m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * this->m_delta_time * 0.1f;
	this->m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * this->m_delta_time * 0.1f;

	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onLeftMouseDown(const Point& delta_mouse_pos)
{
	this->m_scale_cube += 0.5F;
}

void AppWindow::onRightMouseDown(const Point& delta_mouse_pos)
{
	this->m_scale_cube -= 0.5F;
}

void AppWindow::onLeftMouseUp(const Point& delta_mouse_pos)
{
}

void AppWindow::onRightMouseUp(const Point& delta_mouse_pos)
{
}
