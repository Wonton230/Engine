#pragma once

struct ID3D11Buffer;

class IndexBuffer
{
	friend class Renderer;

public:
	IndexBuffer(unsigned int size);
	IndexBuffer(const IndexBuffer& copyFrom) = delete;
	virtual ~IndexBuffer();

	unsigned int GetSize();
	unsigned int GetStride();
	unsigned int GetCount();

private:
	ID3D11Buffer* m_buffer = nullptr;
	unsigned int m_size = 0;
};