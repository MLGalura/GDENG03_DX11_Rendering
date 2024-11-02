#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(RenderSystem* system, const void* shader_byte_code, size_t byte_code_size) : m_system(system)
{
	if (!SUCCEEDED(m_system->m_d3d_Device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &this->m_ps)))
		throw std::exception("PixelShader not created succesfully");
}

PixelShader::~PixelShader()
{
	this->m_ps->Release();
}
