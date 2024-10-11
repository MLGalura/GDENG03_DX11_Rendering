#pragma once
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeviceContext.h"

#include "Vector3D.h"
#include "Matrix4x4.h"

#include <iostream>

class Circle
{
public:
    Circle();
    ~Circle();

public:
    void init(float xVel, float yVel);
    void update(float deltaTime, float windowWidth, float windowHeight);
    void draw();

private:
    Vector3D m_position;
    Vector3D m_velocity;
    Vector3D m_scale;

private:
    float m_radius;
    float m_time;

private:
    ConstantBuffer* m_cb;
    VertexBuffer* m_vb;
    VertexShader* m_vs;
    PixelShader* m_ps;
};
