#include "Particle.h"

struct vec3 {
	float x, y, z;
};

struct vertex
{
    vec3 position;
    vec3 position1;
    vec3 color;
    vec3 color1;
    vec3 xvel;
    vec3 yvel;
};

__declspec(align(16))
struct constant
{
    unsigned int m_time;
};

Particle::Particle(float x, float y, float velX, float velY, float lifetime) : m_lifetime(lifetime)
{
    // Init Pos - Target Pos - Init Color - Target Color - x velocity - y velocity
    vertex list[] = {
        {{x - 0.035f, y - 0.05f, 0.0f}, {x, y, 0.0f}, {0, 0, 1}, {1, 0, 0}, {velX, 0.0f, 0.0f}, {0.0f, velY, 0.0f}},
        {{x - 0.035f, y + 0.05f, 0.0f}, {x, y, 0.0f}, {0, 1, 0}, {1, 1, 0}, {velX, 0.0f, 0.0f}, {0.0f, velY, 0.0f}},
        {{x + 0.035f, y - 0.05f, 0.0f}, {x, y, 0.0f}, {1, 0, 0}, {0, 0, 1}, {velX, 0.0f, 0.0f}, {0.0f, velY, 0.0f}},
        {{x + 0.035f, y + 0.05f, 0.0f}, {x, y, 0.0f}, {1, 1, 1}, {0, 0, 1}, {velX, 0.0f, 0.0f}, {0.0f, velY, 0.0f}}
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

Particle::~Particle()
{
}

void Particle::update()
{
    // CHECK - hard coded for delta time, find function to fix
    m_age += 0.016f;

    // Particle dies if age exceeds lifetime
    if (m_age >= m_lifetime) {
        //this->release();
        return;
    }

    // Update constant buffer
    constant cc;
    cc.m_time = static_cast<unsigned int>(m_age * 1000);
    this->m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

    GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
    GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
    GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
    GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
}

void Particle::release()
{
    this->m_vb->release();
    this->m_cb->release();
    this->m_vs->release();
    this->m_ps->release();
}
