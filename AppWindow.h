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
#include "InputListener.h"

#include "Cube.h"

class AppWindow : public Window, public InputListener
{
public:
	// Constructor & Destructor
	AppWindow();
	void update();
	~AppWindow();

private:
	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

private:
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;

private:
	Cube* m_cube = nullptr;
	constant cc;

private:
	float m_old_delta; // Time when previous frame rendered
	float m_new_delta; // Time when current frame rendered
	float m_delta_time; // difference between old and new

	float m_delta_pos;
	float m_delta_scale;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Matrix4x4 world_cam;
	Matrix4x4 m_world_cam;

	// Inherited via InputListener
	void onKeyDown(int key) override;
	void onKeyUp(int key) override;

	// Inherited via InputListener
	void onMouseMove(const Point& mouse_pos) override;

	// Inherited via Window
	void onFocus() override;
	void onKillFocus() override;

	// Inherited via InputListener
	void onLeftMouseDown(const Point& delta_mouse_pos) override;
	void onRightMouseDown(const Point& delta_mouse_pos) override;
	void onLeftMouseUp(const Point& delta_mouse_pos) override;
	void onRightMouseUp(const Point& delta_mouse_pos) override;
};

