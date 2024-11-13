#include "Texture.h"
#include <DirectXTex.h>
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* full_path) : Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, nullptr, image_data);

	if (SUCCEEDED(res))
		res = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_Device, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &this->m_texture);

	else
		throw std::exception("Tex not created bruh");
}

Texture::~Texture()
{
}
