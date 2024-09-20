#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
public:
	// Constructor & Destructor
	GraphicsEngine();
	~GraphicsEngine();

	// Initializes the Graphics Engine and the DirectX11 Drive
	bool init();

	// Releases all the resources loaded
	bool release();

public:
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();

	bool createShaders();
	bool setShaders();

	void getShaderBufferAndSize(void** bytecode, UINT* size);

public:
	static GraphicsEngine* get();

private:
	DeviceContext* m_imm_device_context;

private:
	ID3D11Device* m_d3d_Device;
	D3D_FEATURE_LEVEL m_feature_level;

private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;

private:
	ID3DBlob* m_vsblob;
	ID3DBlob* m_psblob;

	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;


private:
	friend class SwapChain;
	friend class VertexBuffer;
};

