#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

#include "InputSystem.h"

struct vertex {
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};


AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	cc.m_time = ::GetTickCount();

	this->m_delta_pos += this->m_delta_time / 10.0f;

	if (this->m_delta_pos > 1.0f)
		this->m_delta_pos = 0.0f;

	Matrix4x4 temp;

	// cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2.0f, -2.0f, 0.0f), Vector3D(2.0f, 2.0f, 0), this->m_delta_pos));

	this->m_delta_scale += this->m_delta_time / 1.0f;

	cc.m_world.setIdentity();
	/*
	cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0.5f), Vector3D(1.0f, 1.0f, 1.0f), (sin(this->m_delta_scale) + 1.0f) / 2.0f));

	temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0.0f), Vector3D(1.5f, 1.5f, 0), this->m_delta_pos));

	cc.m_world *= temp;

	cc.m_world.setScale(Vector3D(this->m_scale_cube, this->m_scale_cube, this->m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(this->m_rot_y);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(this->m_rot_x);

	cc.m_world *= temp;*/

	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(this->m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(this->m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = this->m_world_cam.getTranslation() + world_cam.getZDirection() * (this->m_forward * 0.3f);

	new_pos = new_pos + world_cam.getXDirection() * (this->m_rightward * 0.3f);

	world_cam.setTranslation(new_pos);

	this->m_world_cam = world_cam;

	world_cam.inverse();

	cc.m_view = world_cam;
	/*cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);*/

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovLH(1.5f, ((float)width / (float)height), 0.1f, 100.0f);

	this->m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	GraphicsEngine::get()->init();
	RECT rc = this->getClientWindowRect();

	this->m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);
	this->m_world_cam.setTranslation(Vector3D(0.0f, 0.0f, -2.0f));

	/*
	vertex vertex_list[] = {
		// FRONT FACE
		{Vector3D(-0.5f, -0.5f, -0.5f),	Vector3D(1, 0, 0),	Vector3D(0.2f, 0, 0)},
		{Vector3D(-0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 0),	Vector3D(0.2f, 0.2f, 1)},
		{Vector3D(0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 0),	Vector3D(0.2f, 0.2f, 0)},
		{Vector3D(0.5f, -0.5f, -0.5f),	Vector3D(1, 0, 0),	Vector3D(0.2f, 0, 0)},

		// BACK FACE
		{Vector3D(0.5f, -0.5f, 0.5f),	Vector3D(0, 1, 0),	Vector3D(0, 0.2f, 0)},
		{Vector3D(0.5f, 0.5f, 0.5f),	Vector3D(0, 1, 1),	Vector3D(0, 0.2f, 0.2f)},
		{Vector3D(-0.5f, 0.5f, 0.5f),	Vector3D(0, 1, 1),	Vector3D(0, 0.2f, 0.2f)},
		{Vector3D(-0.5f, -0.5f, 0.5f),	Vector3D(0, 1, 0),	Vector3D(0, 0.2f, 0)}
	};

	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		// FRONT SIDE
		0, 1, 2, // FIRST TRIANGLE
		2, 3, 0, // SECOND TRIANGLE

		// BACK SIDE
		4, 5, 6, 
		6, 7, 4, 

		// TOP SIDE
		1, 6, 5, 
		5, 2, 1,

		// BOTTOM SIDE
		7, 0, 3,
		3, 4, 7,

		// RIGHT SIDE
		3, 2, 5,
		5, 4, 3,

		// LEFT SIDE
		7, 6, 1,
		1, 0, 7
	};

	this->m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	this->m_ib->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0; 

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	this->m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();
	
	
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();
	*/
	this->m_cube = new Cube();
	this->m_cube->init(Vector3D(5.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f));

	cc.m_time = 0;

	this->m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
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

	this->m_cube->update(this->m_delta_time, this->cc);
	this->m_cube->draw();

	
	/*GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb); 
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib); 

	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);*/
	

	this->m_swap_chain->present(true);

	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = ::GetTickCount();

	this->m_delta_time = (this->m_old_delta) ? (this->m_new_delta - this->m_old_delta) / 1000.0f : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	GraphicsEngine::get()->release();
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
		this->m_forward = 1.0f;
	//this->m_rot_x += 3.14f * this->m_delta_time;

	else if (key == 'S')
		this->m_forward = -1.0f;
		//this->m_rot_x -= 3.14f * this->m_delta_time;
	
	else if (key == 'A')
		this->m_rightward = -1.0f;
		//this->m_rot_y += 3.14f * this->m_delta_time;

	else if (key == 'D')
		this->m_rightward = 1.0f;
		//this->m_rot_y -= 3.14f * this->m_delta_time;
}

void AppWindow::onKeyUp(int key)
{
	this->m_forward = 0.0f;
	this->m_rightward = 0.0f;
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
