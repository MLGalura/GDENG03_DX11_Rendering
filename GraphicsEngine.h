#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	// Constructor & Destructor
	GraphicsEngine();
	~GraphicsEngine();

	// Initializes the Graphics Engine and the DirectX11 Drive
	bool init();

	// Releases all the resources loaded
	bool release();

	RenderSystem* getRenderSystem();


public:
	static GraphicsEngine* get();

private:
	RenderSystem* m_render_system = nullptr;
};

