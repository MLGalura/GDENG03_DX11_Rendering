#include "Cube.h"
#include "GraphicsEngine.h"

#include <Windows.h>

struct vertex {
	Vector3D position;
	Vector2D texcoord;
};

Cube::Cube()
{

}

Cube::~Cube()
{
}

void Cube::init(Vector3D vel, Vector3D pos)
{
	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Wood.jpg");

	Vector3D position_list[] = {
		// FRONT FACE
		{Vector3D(-0.5f, -0.5f, -0.5f)},
		{Vector3D(-0.5f, 0.5f, -0.5f)},
		{Vector3D(0.5f, 0.5f, -0.5f)},
		{Vector3D(0.5f, -0.5f, -0.5f)},

		// BACK FACE
		{Vector3D(0.5f, -0.5f, 0.5f)},
		{Vector3D(0.5f, 0.5f, 0.5f)},
		{Vector3D(-0.5f, 0.5f, 0.5f)},
		{Vector3D(-0.5f, -0.5f, 0.5f)}
	};

	Vector2D texcoord_list[] = {
		{Vector2D(0.0f, 0.0f)},
		{Vector2D(0.0f, 1.0f)},
		{Vector2D(1.0f, 0.0f)},
		{Vector2D(1.0f, 1.0f)},
	};

	vertex vertex_list[] = {
		// FRONT FACE
		{ position_list[0], texcoord_list[1] },
		{ position_list[1], texcoord_list[0] },
		{ position_list[2], texcoord_list[2] },
		{ position_list[3], texcoord_list[3] },
		
		// BACK FACE
		{ position_list[4], texcoord_list[1] },
		{ position_list[5], texcoord_list[0] },
		{ position_list[6], texcoord_list[2] },
		{ position_list[7], texcoord_list[3] },

		{ position_list[1], texcoord_list[1] },
		{ position_list[6], texcoord_list[0] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[2], texcoord_list[3] },

		{ position_list[7], texcoord_list[1] },
		{ position_list[0], texcoord_list[0] },
		{ position_list[3], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },

		{ position_list[3], texcoord_list[1] },
		{ position_list[2], texcoord_list[0] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },

		{ position_list[7], texcoord_list[1] },
		{ position_list[6], texcoord_list[0] },
		{ position_list[1], texcoord_list[2] },
		{ position_list[0], texcoord_list[3] },
	};

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
		8, 9, 10,
		10, 11, 8,

		// BOTTOM SIDE
		12, 13, 14,
		14, 15, 12,

		// RIGHT SIDE
		16, 17, 18,
		18, 19, 16,

		// LEFT SIDE
		20, 21, 22,
		22, 23, 20
	};

	UINT size_index_list = ARRAYSIZE(index_list);
	this->indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	this->vertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	this->vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();


	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	this->pixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	this->constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void Cube::update(float deltaTime, constant cc2)
{
	constant cc;
	cc.m_time = ::GetTickCount();

	Matrix4x4 temp;
	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(1.0f, 1.0f, 1.0f));

	cc.m_view = cc2.m_view;
	cc.m_proj = cc2.m_proj;

	this->constantBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void Cube::draw()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(this->vertexShader, this->constantBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(this->pixelShader, this->constantBuffer);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(this->vertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(this->pixelShader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(this->pixelShader, this->m_wood_tex);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(this->vertexBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(this->indexBuffer);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}
