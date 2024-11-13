#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeviceContext.h"

#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

#include <iostream>

__declspec(align(16))
struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};


class Cube
{
public:
	Cube();
	~Cube();

public:
	void init(Vector3D vel, Vector3D pos);
	void update(float deltaTime, constant cc2);
	void draw();


private:
	VertexBufferPtr vertexBuffer;
	IndexBufferPtr indexBuffer;
	ConstantBufferPtr constantBuffer;

	VertexShaderPtr vertexShader;
	PixelShaderPtr pixelShader;

	TexturePtr m_wood_tex;

	Vector3D animSpeed;

	Vector3D velocity;
	Vector3D position;
};