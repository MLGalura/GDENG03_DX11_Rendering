#pragma once

#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "Matrix4x4.h"
#include "Cube.h"
#include "Plane.h"
#include <vector>

class AppWindow : public Window
{
public:
	// Constructor & Destructor
	AppWindow();
	void updateQuadPosition();
	~AppWindow();

private:
	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;

private:
	float m_old_delta; // Time when previous frame rendered
	float m_new_delta; // Time when current frame rendered
	float m_delta_time; // difference between old and new

	float m_delta_pos;
	float m_delta_scale;

	std::vector<Cube*> m_cubeList; 
	Plane* m_plane;
};

