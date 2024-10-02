#include "Quad.h"
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

__declspec(align(16))
struct constant {
    unsigned int m_time;
};

Quad::Quad(float x, float y) : m_x(x), m_y(y)
{
	
}

Quad::~Quad()
{
    
}

void Quad::initialize()
{
	vertex list[] = {

		{ {-0.25f + this->m_x, -0.25f + this->m_y, 0.0f}, {-0.25f + this->m_x, -0.25f + this->m_y, 0.0f}, {0, 0, 0}, {0, 1, 0} },
		{ {-0.25f + this->m_x, 0.25f + this->m_y, 0.0f}, {-0.25f + this->m_x, 0.25f + this->m_y, 0.0f}, {1, 1, 0}, {0, 1, 1} },
		{ {0.25f + this->m_x, -0.25f + this->m_y, 0.0f}, {0.25f + this->m_x, -0.25f + this->m_y, 0.0f}, {0, 0, 1}, {1, 0, 0} },
		{ {0.25f + this->m_x, 0.25f + this->m_y, 0.0f}, {0.25f + this->m_x, 0.25f + this->m_y, 0.0f}, {1, 1, 1}, {0, 0, 1} }
	};

	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	this->m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	this->m_cb = GraphicsEngine::get()->createConstantBuffer();
	this->m_cb->load(&cc, sizeof(constant));
}

void Quad::update()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	this->m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
}

void Quad::release()
{
	this->m_vb->release();
	this->m_cb->release();
	this->m_vs->release();
	this->m_ps->release();
}
