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

	vertex list[] = {
		{Vector3D(-0.5f, -0.5f, 0.0f), Vector3D(-0.32f, -0.11f, 0.0f), Vector3D(0, 0, 0), {0.0f, 0.0f}},
		{Vector3D(-0.5f,  0.5f, 0.0f), Vector3D(-0.11f,  0.78f, 0.0f), Vector3D(1, 1, 0), {0.0f, 1.0f}},
		{Vector3D(0.5f, -0.5f, 0.0f), Vector3D(0.75f, -0.73f, 0.0f), Vector3D(0, 0, 1), {1.0f, 0.0f}},
		{Vector3D(0.5f,  0.5f, 0.0f), Vector3D(0.88f,  0.77f, 0.0f), Vector3D(1, 1, 1), {1.0f, 1.0f}}
	};


	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list); 

	// GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0; 

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	this->m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();
	
	
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);

	constant cc;
	cc.m_time = 0;

	this->m_cb = GraphicsEngine::get()->createConstantBuffer();
	this->m_cb->load(&cc, sizeof(constant) );
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top); 
	// GraphicsEngine::get()->setShaders();
	

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb); 
	// GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0); // OLD RECTANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);

	this->m_swap_chain->present(true);

	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = ::GetTickCount();

	this->m_delta_time = (this->m_old_delta) ? (this->m_new_delta - this->m_old_delta) / 1000.0f : 0;

	this->updateQuadPosition();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	this->m_vb->release();
	this->m_swap_chain->release();
	this->m_vs->release();
	this->m_ps->release();
	GraphicsEngine::get()->release();
}
