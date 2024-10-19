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

class Plane
{
public:
	Plane();
	~Plane();

public:
	void init(Vector3D pos);
	void update(float deltaTime, float windowWidth, float windowHeight);
	void draw();

private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Vector3D position;
	float deltaPos;
};

