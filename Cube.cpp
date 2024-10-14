#include "Cube.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"

struct vertex {
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

Cube::Cube(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
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
}

Cube::~Cube()
{
}

void Cube::update(float deltaTime)
{
}

void Cube::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
}

void Cube::setAnimSpeed(float speed)
{
}
