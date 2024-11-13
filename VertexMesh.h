#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public:
	VertexMesh() : m_position(), m_texcoords() {

	}

	VertexMesh(Vector3D position, Vector2D texcoords) : m_position(position), m_texcoords(texcoords) {

	}

	VertexMesh(const VertexMesh& vertex) : m_position(vertex.m_position), m_texcoords(vertex.m_texcoords) {

	}

	~VertexMesh() {

	}

public:
	Vector3D m_position;
	Vector2D m_texcoords;
};