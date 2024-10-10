#include "VertexBuffer.h"
#include "GraphicsEngine.h"

VertexBuffer::VertexBuffer() : m_Layout(0), m_buffer(0)
{
}

VertexBuffer::~VertexBuffer()
{
}



bool VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	if (this->m_buffer)this->m_buffer->Release();
	if (this->m_Layout)this->m_Layout->Release();

	D3D11_BUFFER_DESC buff_desc = {}; 
	buff_desc.Usage = D3D11_USAGE_DEFAULT; 
	buff_desc.ByteWidth = size_vertex * size_list; 
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	buff_desc.CPUAccessFlags = 0; 
	buff_desc.MiscFlags = 0; 

	D3D11_SUBRESOURCE_DATA init_data = {}; 
	init_data.pSysMem = list_vertices; 

	this->m_size_vertex = size_vertex; 
	this->m_size_list = size_list;

	if (FAILED(GraphicsEngine::get()->m_d3d_Device->CreateBuffer(&buff_desc, &init_data, &m_buffer))) {
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		// SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALGINED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(GraphicsEngine::get()->m_d3d_Device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_Layout))) {
		return false;
	}

	return true;
}

bool VertexBuffer::release()
{
	this->m_Layout->Release();
	this->m_buffer->Release();
	delete this;
	return true;
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}
