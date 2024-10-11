#include "Circle.h"
#include "GraphicsEngine.h"
#include <Windows.h>

struct vertex {
    Vector3D position;
    Vector3D color;
    Vector3D color1;
};

__declspec(align(16))
struct constant {
    Matrix4x4 m_world;
    Matrix4x4 m_view;
    Matrix4x4 m_proj;
    unsigned int m_time;
};

Circle::Circle() : m_radius(0.15f), m_position(0, 0, 0), m_scale(0.5f, 0.5f, 0.5f), m_time(0)
{
}

Circle::~Circle()
{
    m_cb->release();
    m_vb->release();
    m_vs->release();
    m_ps->release();
}

void Circle::init(float xVel, float yVel)
{
    this->m_velocity.m_x = xVel;
    this->m_velocity.m_y = yVel;

    vertex list[] = {
        {Vector3D(-0.5f, -0.5f, 0.0f), Vector3D(-0.32f, -0.11f, 0.0f), Vector3D(0, 0, 0)},
        {Vector3D(-0.5f, 0.5f, 0.0f), Vector3D(-0.11f, 0.78f, 0.0f), Vector3D(1, 1, 0)},
        {Vector3D(0.5f, -0.5f, 0.0f), Vector3D(0.75f, -0.73f, 0.0f), Vector3D(0, 0, 1)},
        {Vector3D(0.5f, 0.5f, 0.0f), Vector3D(0.88f, 0.77f, 0.0f), Vector3D(1, 1, 1)}
    };

    m_vb = GraphicsEngine::get()->createVertexBuffer();
    UINT size_list = ARRAYSIZE(list);

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
    m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
    m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

    GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
    m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

    constant cc;
    cc.m_time = 0;

    m_cb = GraphicsEngine::get()->createConstantBuffer();
    m_cb->load(&cc, sizeof(constant));
}

void Circle::update(float deltaTime, float windowWidth, float windowHeight)
{
    // Update position
    m_position.m_x += m_velocity.m_x * deltaTime;
    m_position.m_y += m_velocity.m_y * deltaTime;


    // Check boundaries and reverse direction if necessary
    if (m_position.m_x + m_radius >= (windowWidth / 400.0f) / 2.0f || m_position.m_x - m_radius <= -(windowWidth / 400.0f) / 2.0f)
        m_velocity.m_x = -m_velocity.m_x;
    if (m_position.m_y + m_radius >= (windowHeight / 400.0f) / 2.0f || m_position.m_y - m_radius <= -(windowHeight / 400.0f) / 2.0f)
        m_velocity.m_y = -m_velocity.m_y;

    // Update transformation matrices
    constant cc;
    cc.m_time = ::GetTickCount();

    Matrix4x4 translationMatrix;
    translationMatrix.setTranslation(m_position);

    cc.m_world.setScale(Vector3D(0.5f, 0.5f, 0.5f));

    cc.m_world *= translationMatrix;
    cc.m_view.setIdentity();
    cc.m_proj.setOrthoLH(windowWidth / 400.0f, windowHeight / 400.0f, -4.0f, 4.0f);

    m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void Circle::draw()
{
    GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
    GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
    GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

    GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
    GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
}