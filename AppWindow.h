#pragma once

#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"

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
};

