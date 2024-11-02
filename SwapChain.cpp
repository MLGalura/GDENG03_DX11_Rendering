#include "SwapChain.h"
#include "RenderSystem.h"

SwapChain::SwapChain(RenderSystem* system) : m_system(system)
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
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
		return false;
	}

	ID3D11Texture2D* buffer = NULL;
	HRESULT bhr = this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr)) {
		return false;
	}

	HRESULT rhr = device->CreateRenderTargetView(buffer, NULL, &this->m_rtv);
	buffer->Release();

	if (FAILED(hr)) {
		return false;
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
	if (FAILED(dhr)) return false;

	HRESULT dshr = device->CreateDepthStencilView(buffer, NULL, &this->m_dsv);
	if (FAILED(dshr)) return false;
	buffer->Release();

	ID3D11DeviceContext* context = m_system->m_imm_context;
	context->OMSetRenderTargets(1, &m_rtv, m_dsv);

	return true;
}

bool SwapChain::present(bool vsync)
{
	this->m_swap_chain->Present(vsync, NULL);
	return true;
}

bool SwapChain::release()
{
	this->m_swap_chain->Release();
	delete this;
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
