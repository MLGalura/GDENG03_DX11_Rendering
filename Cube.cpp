#include "Cube.h"
#include "GraphicsEngine.h"

#include <Windows.h>

struct vertex {
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

Cube::Cube()
{

}

Cube::~Cube()
{
}

void Cube::init(Vector3D vel, Vector3D pos, Vector3D sca)
{
	this->velocity = vel;
	this->position = pos;
	this->scale = sca;

	vertex vertex_list[] = {
		// FRONT FACE
		{Vector3D(-0.5f, -0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(0.5f, -0.5f, -0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},

		// BACK FACE
		{Vector3D(0.5f, -0.5f, 0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(0.5f, 0.5f, 0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.5f, 0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, -0.5f, 0.5f),	Vector3D(1, 1, 1),	Vector3D(1, 1, 1)}
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

void Cube::update(float deltaTime, constant cc2)
{
	constant cc;
	cc.m_time = ::GetTickCount();

	this->m_delta_sca += deltaTime / 1.0f;

	Matrix4x4 temp;
	cc.m_world.setIdentity();
	cc.m_world.setScale(this->scale);

	Matrix4x4 rotMatrix;
	Matrix4x4 translationMatrix;

	// THE ANIM ISNT BEING USED WHICH IS FO R USUAL ROTATION
	this->animSpeed.m_x += deltaTime * this->velocity.m_x;
	this->animSpeed.m_y += deltaTime * this->velocity.m_y;
	this->animSpeed.m_z += deltaTime * this->velocity.m_z;

	rotMatrix.setIdentity();
	rotMatrix.setRotationZ(this->velocity.m_z);
	cc.m_world *= rotMatrix;
	
	rotMatrix.setIdentity();
	rotMatrix.setRotationY(this->velocity.m_y);
	cc.m_world *= rotMatrix;

	rotMatrix.setIdentity();
	rotMatrix.setRotationX(this->velocity.m_x);
	cc.m_world *= rotMatrix;

	translationMatrix.setIdentity();
	translationMatrix.setTranslation(this->position);

	cc.m_world *= translationMatrix;

	cc.m_view = cc2.m_view;
	cc.m_proj = cc2.m_proj;

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
