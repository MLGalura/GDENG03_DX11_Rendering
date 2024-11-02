#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>

SwapChain::SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height) : m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_Device;

	// Chain descriptor that handle the angle, size of the window
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &this->m_swap_chain);

	if (FAILED(hr)) {
		throw std::exception("SwapChain not created successfully");
	}

	ID3D11Texture2D* buffer = NULL;
	HRESULT bhr = this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr)) {
		throw std::exception("SwapChain not created successfully");
	}

	HRESULT rhr = device->CreateRenderTargetView(buffer, NULL, &this->m_rtv);
	buffer->Release();

	if (FAILED(hr)) {
		throw std::exception("SwapChain not created successfully");
	}

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.CPUAccessFlags = 0;

	HRESULT dhr = device->CreateTexture2D(&texDesc, NULL, &buffer);
	if (FAILED(dhr)) throw std::exception("SwapChain not created successfully");

	HRESULT dshr = device->CreateDepthStencilView(buffer, NULL, &this->m_dsv);
	if (FAILED(dshr)) throw std::exception("SwapChain not created successfully");
	buffer->Release();

	ID3D11DeviceContext* context = m_system->m_imm_context;
	context->OMSetRenderTargets(1, &m_rtv, m_dsv);
}

SwapChain::~SwapChain()
{
	this->m_rtv->Release();
	this->m_dsv->Release(); 
	this->m_swap_chain->Release();
}

bool SwapChain::present(bool vsync)
{
	this->m_swap_chain->Present(vsync, NULL);
	return true;
}

ID3D11RenderTargetView* SwapChain::getRenderTargetView()
{
	return this->m_rtv;
}

ID3D11DepthStencilView* SwapChain::getDepthStencilView()
{
	return this->m_dsv;
}
