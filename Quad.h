#pragma once
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"

class Quad
{
public:
    Quad(float x, float y); 
    ~Quad();

public:
    void initialize(); 
    void update();
    void release();  

private:
    float m_x;
    float m_y;

private:
    VertexBuffer* m_vb;
    ConstantBuffer* m_cb;
    VertexShader* m_vs;
    PixelShader* m_ps;
};

