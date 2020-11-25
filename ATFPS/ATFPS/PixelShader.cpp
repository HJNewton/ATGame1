#include "PixelShader.h"

PixelShader::PixelShader(Graphics& GFX, const std::wstring& path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	GetDevice(GFX)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
}

void PixelShader::Bind(Graphics& GFX) noexcept
{
	GetContext(GFX)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}