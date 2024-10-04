#pragma once

#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class AppWindow : public Window
{
public:
	// Constructor & Destructor
	AppWindow();
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
	double accumulatedTime = 0.0f;
	double elapsedTime = 0.0f;

	float curSpeed = 1.0f;
	float maxSpeed = 5.0f;
	float minSpeed = 0.5f;
	float lerpSpeed = 2.5f;

	bool isIncreasing = true;
};

