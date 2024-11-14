#pragma once

#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "Prerequisites.h"

#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Texture.h"

#include <string>
#include <vector>
#include <exception>

class GameObject
{
public:
	GameObject(std::string name);
	~GameObject();

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(Matrix4x4 view, Matrix4x4 proj) = 0;

public:
	std::string GetName();

	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3D position);
	Vector3D GetLocalPosition() const;

	void SetScale(float x, float y, float z);
	void SetScale(Vector3D scale);
	Vector3D GetLocalScale() const;

	void SetRotation(float x, float y, float z);
	void SetRotation(Vector3D rotation);
	Vector3D GetLocalRotation() const;

	void SetTransform(const Matrix4x4& transform);

	Vector3D GetRight();
	Vector3D GetUp();
	Vector3D GetForward();

	Matrix4x4 GetTransform();
	void MultiplyTransform(Matrix4x4 transform);
	GameObject* GetParent();
	GameObject* GetChild(int index);
	std::vector<GameObject*> GetChildren();

	virtual void SetTexture(const wchar_t* file_path);

	bool IsEnabled() const;
	void setEnabled(bool enabled);

public:
	PixelShaderPtr GetPixelShader();
	VertexShaderPtr GetVertexShader();
	TexturePtr GetTexture();

	void SetPixelShader(PixelShaderPtr pixelShader);
	void SetVertexShader(VertexShaderPtr vertexShader);

protected:
	std::string name;

	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;

	Matrix4x4 transform;
	TexturePtr tex;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool enabled;

	VertexShaderPtr vertexShader;
	PixelShaderPtr pixelShader;
};
