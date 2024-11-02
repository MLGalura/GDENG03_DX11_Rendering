#include "GraphicsEngine.h"
#include "RenderSystem.h"

#include <d3dcompiler.h>

GraphicsEngine::GraphicsEngine()
{
	
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	this->m_render_system = new RenderSystem();
	this->m_render_system->init();
	return true;
}

bool GraphicsEngine::release()
{
	this->m_render_system->release();
	delete this->m_render_system;
	return true;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return this->m_render_system;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
