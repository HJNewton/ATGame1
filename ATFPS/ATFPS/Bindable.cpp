#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& GFX) noexcept
{
	return GFX.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& GFX) noexcept
{
	return GFX.pDevice.Get();
}