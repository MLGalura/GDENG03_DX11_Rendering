#pragma once
#include "Prerequisites.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "GameObject.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "Texture.h"
#include "Mesh.h"

#include "Vector2D.h"
#include "Structs.h"

#include <iostream>

class Object : public GameObject
{
public:
	Object(std::string name, void* shaderBytes, size_t shaderSize);
	~Object();

	void Update(float deltaTime) override;
	void Draw(Matrix4x4 view, Matrix4x4 proj) override;

	void SetTexture(const wchar_t* file_path) override;
	void SetMesh(const wchar_t* file_path);

private:
	VertexBufferPtr vertexBuffer;
	IndexBufferPtr indexBuffer;
	ConstantBufferPtr constantBuffer;

	TexturePtr tex;
	MeshPtr mesh; 

	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaRot = 0.0f;
	float deltaTime = 0.0f;
	float speed = 1.0f;
};