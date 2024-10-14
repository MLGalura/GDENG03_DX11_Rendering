#pragma once
#include "Vector3D.h"
#include <string>

using namespace std;

class VertexShader;
class PixelShader;

class AGameObject
{
public:
	AGameObject(string name);
	~AGameObject();

public:
	virtual void update(float deltaTime) = 0;
	virtual void draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader) = 0;

public:
	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rot);
	Vector3D getLocalRotation();

private:
	string name;
	Vector3D localRotation;
	Vector3D localScale;
	Vector3D localPosition;
};

