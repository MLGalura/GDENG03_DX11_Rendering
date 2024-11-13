#include "GraphicsEngine.h"
#include "RenderSystem.h"

#include <d3dcompiler.h>

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		this->m_render_system = new RenderSystem();
	}
	catch (...) { throw std::exception("Render System not made successfully"); }
}

GraphicsEngine::~GraphicsEngine()
{
	delete this->m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return this->m_render_system;
}

GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine)
		throw std::exception("Graphics Engine already exists");

	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine) return;

	delete GraphicsEngine::m_engine; 
}
