#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	// Initialization and release of SwapChain	
	bool init(HWND hwnd, UINT width, UINT height);
	bool present(bool vsync);
	bool release();

public:
	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;

	ID3D11Texture2D* m_depth_stencil_buffer;
	ID3D11DepthStencilView* m_dsv;

private:
	friend class DeviceContext;
};

