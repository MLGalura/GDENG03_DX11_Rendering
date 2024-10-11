#pragma once

#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "Vector3D.h"
#include "Circle.h"
#include <iostream>

class AppWindow : public Window
{
public:
	// Constructor & Destructor
	AppWindow();
	void updateQuadPosition();
	void checkBorders();
	~AppWindow();

private:
	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;

private:
	float m_old_delta; // Time when previous frame rendered
	float m_new_delta; // Time when current frame rendered
	float m_delta_time; // difference between old and new

	float m_delta_pos;
	float m_delta_scale;

private:
	Vector3D m_pos = Vector3D(0.0f, 0.0f, 0.0f);
	Vector3D m_velocity = Vector3D(0.1f, 0.1f, 0.0f);
	float m_speed = 2.5f;

private:
	Circle m_circle;

};

