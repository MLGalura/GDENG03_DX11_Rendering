#include "Plane.h"
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

Plane::Plane()
{
}

Plane::~Plane()
{
}

void Plane::init(Vector3D pos)
{
	vertex vertex_list[] = {
		// FRONT FACE
		{Vector3D(-0.5f, -0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(0.5f, -0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},

		// BACK FACE
		{Vector3D(-0.5f, -0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(0.5f, -0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)}
	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
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

	this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	this->indexBuffer->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	this->vertexShader = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	this->vertexBuffer->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	this->pixelShader = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();


	constant cc;
	cc.m_time = 0;

	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cc, sizeof(constant));
}

void Plane::update(float deltaTime, float windowWidth, float windowHeight)
{
	constant cc;
	cc.m_time = ::GetTickCount();

	cc.m_world.setScale(Vector3D(0.75f, 0.75f, 0.25f));

	this->deltaPos += deltaTime / 1.0f;

	Matrix4x4 translationMatrix;
	Matrix4x4 rotationMatrix;

	translationMatrix.setIdentity();
	translationMatrix.setTranslation(Vector3D::lerp(Vector3D(-0.25f, 0.0f, 0.0f), Vector3D(0.25f, 0.0f, 0), (sin(this->deltaPos) + 1.0f) / 2.0f));

	//rotationMatrix.setIdentity();
	//rotationMatrix.setRotationY(this->deltaPos * 10);

	//cc.m_world *= rotationMatrix;

	rotationMatrix.setIdentity();
	rotationMatrix.setRotationX(this->deltaPos);

	cc.m_world *= rotationMatrix;

	cc.m_world *= translationMatrix;
	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH(windowWidth / 400.0f, windowHeight / 400.0f, -4.0f, 4.0f);

	this->constantBuffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void Plane::draw()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->vertexShader, this->constantBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->pixelShader, this->constantBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->pixelShader);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(this->vertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(this->indexBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}
