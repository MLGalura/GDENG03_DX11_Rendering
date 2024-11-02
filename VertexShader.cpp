#include "VertexShader.h"
#include "RenderSystem.h"
#include <exception>


VertexShader::VertexShader(RenderSystem* system, const void* shader_byte_code, size_t byte_code_size) : m_system(system)
{
	if (!SUCCEEDED(m_system->m_d3d_Device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &this->m_vs)))
		throw std::exception("VertexShader not created successfully");
}

VertexShader::~VertexShader()
{
	this->m_vs->Release();
}