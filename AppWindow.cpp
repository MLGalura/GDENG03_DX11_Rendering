#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

struct vertex {
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

// DX11 handles vid memory in chunks of 16 bytes, anything more, make sure its aligned or a multiple of 16
__declspec(align(16))
struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;

	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	this->m_delta_pos += this->m_delta_time / 10.0f;

	if (this->m_delta_pos > 1.0f)
		this->m_delta_pos = 0.0f;

	Matrix4x4 temp;

	// cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2.0f, -2.0f, 0.0f), Vector3D(2.0f, 2.0f, 0), this->m_delta_pos));

	this->m_delta_scale += this->m_delta_time / 1.0f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0.0f), Vector3D(1.0f, 1.0f, 0), (sin(this->m_delta_scale) + 1.0f) / 2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0.0f), Vector3D(1.5f, 1.5f, 0), this->m_delta_pos));

	//cc.m_world *= temp;

	cc.m_world.setScale(Vector3D(1, 1, 1));

	temp.setIdentity();
	temp.setRotationZ(m_delta_scale);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_delta_scale);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_delta_scale);

	cc.m_world *= temp;

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);

	this->m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
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

	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);

	constant cc;
	cc.m_time = 0;

	this->m_cb = GraphicsEngine::get()->createConstantBuffer();
	this->m_cb->load(&cc, sizeof(constant) );
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top); 
	// GraphicsEngine::get()->setShaders();
	
	this->updateQuadPosition();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb); 
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib); 

	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	this->m_swap_chain->present(true);

	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = ::GetTickCount();

	this->m_delta_time = (this->m_old_delta) ? (this->m_new_delta - this->m_old_delta) / 1000.0f : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	this->m_vb->release();
	this->m_ib->release();
	this->m_cb->release();
	this->m_swap_chain->release();
	this->m_vs->release();
	this->m_ps->release();
	GraphicsEngine::get()->release();
}
