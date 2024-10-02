#pragma once
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"

#include <vector>
#include <iostream>

class Particle
{
public:
	Particle(float x, float y, float velX, float velY, float lifetime);
	~Particle();

public:
	void update();
	void release();

private:
    // Particle lifetime
    float m_lifetime;
    float m_age = 0.0f;

private:
    VertexBuffer* m_vb;
    ConstantBuffer* m_cb;
    VertexShader* m_vs;
    PixelShader* m_ps;
};

