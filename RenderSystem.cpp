#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"

#include <d3dcompiler.h>
#include <exception>

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{
}

bool RenderSystem::init()
{
	D3D_DRIVER_TYPE driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0 };

	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;) {
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels,
			D3D11_SDK_VERSION, &this->m_d3d_Device, &this->m_feature_level, &m_imm_context);

		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}

	if (FAILED(res)) {
		return false;
	}

	m_imm_device_context = new DeviceContext(m_imm_context, this);

	this->m_d3d_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&this->m_dxgi_device);
	this->m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&this->m_dxgi_adapter);
	this->m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&this->m_dxgi_factory);

	return true;
}

bool RenderSystem::release()
{
	if (m_vs)m_vs->Release();
	if (m_ps)m_ps->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	this->m_dxgi_device->Release();
	this->m_dxgi_adapter->Release();
	this->m_dxgi_factory->Release();

	delete m_imm_device_context;
	this->m_d3d_Device->Release();
	return true;
}

SwapChain* RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChain* sc = nullptr;

	try {
		sc = new SwapChain(this, hwnd, width, height);
	}

	catch (...) {}

	return sc;
}

DeviceContext* RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	VertexBuffer* vb = nullptr;

	try {
		vb = new VertexBuffer(this, list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader);
	}

	catch (...) {}

	return vb;
}

IndexBuffer* RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBuffer* ib = nullptr;

	try {
		ib = new IndexBuffer(this, list_indices, size_list);
	}
	catch (...) {}

	return ib;;
}

ConstantBuffer* RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBuffer* cb = nullptr;

	try {
		cb = new ConstantBuffer(this, buffer, size_buffer);
	}
	catch (...) {}
	
	return cb;
}

VertexShader* RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = nullptr;

	try {
		vs = new VertexShader(this, shader_byte_code, byte_code_size);
	}
	catch (...) {}
	return vs;
}

PixelShader* RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = nullptr;

	try {
		ps = new PixelShader(this, shader_byte_code, byte_code_size);
	}
	catch (...) {}
	return ps;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;

	if (!SUCCEEDED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &this->m_blob, &error_blob))) {
		if (error_blob)
			error_blob->Release();

		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = this->m_blob->GetBufferSize();

	return true;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;

	if (!SUCCEEDED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &this->m_blob, &error_blob))) {
		if (error_blob)
			error_blob->Release();

		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = this->m_blob->GetBufferSize();

	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if (this->m_blob)
		this->m_blob->Release();
}
