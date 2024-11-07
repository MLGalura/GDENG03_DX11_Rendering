#pragma once
#include "UIScreen.h"
#include <d3d11.h>

class Camera;

class InspectorScreen : public UIScreen
{
protected:
	InspectorScreen();
	~InspectorScreen();
	virtual void DrawUI() override;

	friend class UIManager;

private:
	//THESE ARE TESTERS
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11ShaderResourceView* srv = nullptr;
};