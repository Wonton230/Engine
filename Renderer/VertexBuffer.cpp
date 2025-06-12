#include "VertexBuffer.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#if defined(_DEBUG)
#define ENGINE_DEBUG_RENDER
#endif

#if defined(ENGINE_DEBUG_RENDER)
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

VertexBuffer::VertexBuffer(ID3D11Device* device, unsigned int size, unsigned int stride)
{
	m_device = device;
	m_size = size * stride;
	m_stride = stride;
	Create();
}

VertexBuffer::~VertexBuffer()
{
	m_device = nullptr;
	DX_SAFE_RELEASE(m_buffer);
}

void VertexBuffer::Create()
{
	//Create Vertex Buffer
	HRESULT hr;
	UINT vertexBufferSize = m_size;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = vertexBufferSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex buffer");
	}
}

void VertexBuffer::Resize(unsigned int size)
{
	DX_SAFE_RELEASE(m_buffer);
	m_size = size;
	Create();
}

unsigned int VertexBuffer::GetSize()
{
	return m_size;
}

unsigned int VertexBuffer::GetStride()
{
	return m_stride;
}
