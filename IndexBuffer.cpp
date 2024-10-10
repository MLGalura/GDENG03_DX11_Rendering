#include "IndexBuffer.h"
#include "GraphicsEngine.h"

IndexBuffer::IndexBuffer() : m_buffer(0)
{
}

IndexBuffer::~IndexBuffer()
{
}



bool IndexBuffer::load(void* list_indices, UINT size_list)
{
	if (this->m_buffer)this->m_buffer->Release();

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = 4 * size_list; // Each element has a size of 4 bytes
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	this->m_size_list = size_list;

	if (FAILED(GraphicsEngine::get()->m_d3d_Device->CreateBuffer(&buff_desc, &init_data, &m_buffer))) {
		return false;
	}

	return true;
}

bool IndexBuffer::release()
{
	this->m_buffer->Release();
	delete this;
	return true;
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}
