#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& GFX) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(Graphics& GFX) noexcept;
	static ID3D11Device* GetDevice(Graphics& GFX) noexcept;
	//static ID3D11Device* GetDevice(Graphics& GFX) noexcept;
};