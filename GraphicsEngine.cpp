#include "GraphicsEngine.h"
#include "RenderSystem.h"

#include <d3dcompiler.h>

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try	{ this->m_render_system = new RenderSystem(); }
	catch (...) { throw std::exception("Render System not made successfully"); }

	try { this->m_tex_manager = new TextureManager(); }
	catch (...) { throw std::exception("Texture Manager not made successfully"); }

	try { this->m_mesh_manager = new MeshManager(); }
	catch (...) { throw std::exception("Mesh Manager not made successfully"); }

	// To be moved
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	this->m_render_system->compileVertexShader(L"VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);
	m_mesh_layout_size = size_shader;

	this->m_render_system->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine() 
{
	GraphicsEngine::m_engine = nullptr;
	delete this->m_mesh_manager;
	delete this->m_tex_manager;
	delete this->m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return this->m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return this->m_tex_manager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return this->m_mesh_manager;
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	// To be moved
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
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
