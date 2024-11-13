#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system) : m_device_context(device_context), m_system(system)
{

}

DeviceContext::~DeviceContext()
{
	this->m_device_context->Release();
}

void DeviceContext::clearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	ID3D11RenderTargetView* renderView = swap_chain->getRenderTargetView();
	ID3D11DepthStencilView* depthView = swap_chain->getDepthStencilView();
	this->m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	this->m_device_context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	this->m_device_context->OMSetRenderTargets(1, &renderView, depthView);
}

void DeviceContext::setVertexBuffer(VertexBufferPtr vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;

	this->m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	this->m_device_context->IASetInputLayout(vertex_buffer->m_Layout);
}

void DeviceContext::setIndexBuffer(IndexBufferPtr index_buffer)
{

	this->m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	this->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	this->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
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

void DeviceContext::setVertexShader(VertexShaderPtr vertex_shader)
{
	this->m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShaderPtr pixel_shader)
{
	this->m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::setConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer)
{
	this->m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer)
{
	this->m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

ID3D11DeviceContext* DeviceContext::GetDeviceContext()
{
	return this->m_device_context;
}
