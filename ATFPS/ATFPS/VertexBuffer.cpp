#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& GFX) noexcept
{
	const UINT offset = 0u;
	GetContext(GFX)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}