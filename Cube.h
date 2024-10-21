#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeviceContext.h"

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
	void init(Vector3D vel, Vector3D pos, Vector3D sca);
	void update(float deltaTime, constant cc2);
	void draw();


private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Vector3D animSpeed;
	float m_delta_pos;
	float m_delta_sca;

	Vector3D velocity;
	Vector3D position;
	Vector3D scale;
};