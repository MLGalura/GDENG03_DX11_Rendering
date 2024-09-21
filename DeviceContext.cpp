#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context) : m_device_context(device_context)
{

}

DeviceContext::~DeviceContext()
{
}

bool DeviceContext::release()
{
	this->m_device_context->Release();
	delete this;
	return true;
}

void DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = {red, green, blue, alpha};
	this->m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	this->m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, NULL);
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;

	this->m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	this->m_device_context->IASetInputLayout(vertex_buffer->m_Layout);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	this->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	this->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Height = height;
	vp.Width = width;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	this->m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(VertexShader* vertex_shader)
{
	this->m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShader* pixel_shader)
{
	this->m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}
