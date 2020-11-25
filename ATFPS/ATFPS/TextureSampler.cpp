#include "TextureSampler.h"

TextureSampler::TextureSampler(Graphics& GFX)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GetDevice(GFX)->CreateSamplerState(&samplerDesc, &pSampler);
}

void TextureSampler::Bind(Graphics& GFX) noexcept
{
	GetContext(GFX)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}