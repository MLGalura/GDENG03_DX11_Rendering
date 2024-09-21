#include "AppWindow.h"

struct vec3 {
	float x, y, z;
};

struct vertex {
	vec3 position;
	vec3 color;
};

AppWindow::AppWindow()
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

	vertex list[] = {

		{-0.5f, -0.5f, 0.0f,	1, 0, 0},
		{-0.5f, 0.5f, 0.0f,		0, 1, 0},
		{0.5f, -0.5f, 0.0f,		0, 0, 1},
		{0.5f, 0.5f, 0.0f,		1, 1, 0}

		/* OLD RECTANGLE
		{-0.5f, -0.5f, 0.0f}, // POS 1
		{-0.5f, 0.5f, 0.0f}, // POS 2
		{0.5f, 0.5f, 0.0f}, // POS 3

		{0.5f, 0.5f, 0.0f}, // POS 1
		{0.5f, -0.5f, 0.0f}, // POS 2
		{-0.5f, -0.5f, 0.0f} // POS 3
		*/
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
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top); 
	// GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb); 
	// GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0); // OLD RECTANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);

	this->m_swap_chain->present(true); //false??
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
