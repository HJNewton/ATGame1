#include "VertexShader.h"

VertexShader::VertexShader(Graphics& GFX, const std::wstring& path)
{
	D3DReadFileToBlob(path.c_str(), &pBytecodeBlob);

	GetDevice(GFX)->CreateVertexShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);
}

void VertexShader::Bind(Graphics& GFX) noexcept
{
	GetContext(GFX)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}