#include "Cube.h"
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

Cube::Cube()
{

}

Cube::~Cube()
{
}

void Cube::init(Vector3D vel, Vector3D pos)
{
	this->velocity.m_x = vel.m_x;
	this->velocity.m_y = vel.m_y;
	this->velocity.m_z = vel.m_z;

	this->position.m_x = pos.m_x;
	this->position.m_y = pos.m_y;
	this->position.m_z = pos.m_z;

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

void Cube::update(float deltaTime, float windowWidth, float windowHeight)
{
	Vector3D rotSpeed;

	this->animSpeed.m_x += deltaTime * this->velocity.m_x;
	this->animSpeed.m_y += deltaTime * this->velocity.m_y;
	this->animSpeed.m_z += deltaTime * this->velocity.m_z;

	constant cc;
	cc.m_time = ::GetTickCount();

	cc.m_world.setScale(Vector3D(0.25f, 0.55f, 0.25f));

	Matrix4x4 rotationMatrix;
	Matrix4x4 translationMatrix;

	rotationMatrix.setIdentity();
	rotationMatrix.setRotationZ(this->animSpeed.m_z);

	cc.m_world *= rotationMatrix;

	rotationMatrix.setIdentity();
	rotationMatrix.setRotationY(this->animSpeed.m_y);

	cc.m_world *= rotationMatrix;

	rotationMatrix.setIdentity();
	rotationMatrix.setRotationX(this->animSpeed.m_x);

	cc.m_world *= rotationMatrix;


	translationMatrix.setIdentity();
	translationMatrix.setTranslation(this->position);

	cc.m_world *= translationMatrix;
	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH(windowWidth / 400.0f, windowHeight / 400.0f, -4.0f, 4.0f);

	this->constantBuffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void Cube::draw()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->vertexShader, this->constantBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->pixelShader, this->constantBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->pixelShader);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(this->vertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(this->indexBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}
