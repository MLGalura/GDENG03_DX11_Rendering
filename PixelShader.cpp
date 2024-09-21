#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader()
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::release()
{
	this->m_ps->Release();
	delete this;
}

bool PixelShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_Device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &this->m_ps)))
		return false;

	return true;
}
