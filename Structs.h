#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"


struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 proj;
	float time;
};