#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& GFX, const class Surface& s);
	void Bind(Graphics& GFX) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};