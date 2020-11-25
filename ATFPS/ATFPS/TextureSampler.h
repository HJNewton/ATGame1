#pragma once
#include "Bindable.h"

class TextureSampler : public Bindable
{
public:
	TextureSampler(Graphics& GFX);
	void Bind(Graphics& GFX) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};