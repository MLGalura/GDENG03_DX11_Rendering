#include "SwapChain.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->m_d3d_Device;

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
	 

	HRESULT hr = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &this->m_swap_chain);

	if (FAILED(hr)) {
		return false;
	}

	ID3D11Texture2D* buffer = NULL;
	hr = this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr)) {
		return false;
	}

	hr = device->CreateRenderTargetView(buffer, NULL, &this->m_rtv);
	buffer->Release();

	if (FAILED(hr)) {
		return false;
	}

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
