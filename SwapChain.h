#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(RenderSystem* system, HWND hwnd, UINT width, UINT height);
	~SwapChain();

	bool present(bool vsync);

public:
	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;

	ID3D11Texture2D* m_depth_stencil_buffer;
	ID3D11DepthStencilView* m_dsv;
	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

