#include "Camera.h"
#include "HarryMath.h"

namespace dx = DirectX;

Camera::Camera() noexcept
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	return dx::XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * dx::XMMatrixRotationRollPitchYaw(0.0f, -yaw, 0.0f);
}

void Camera::Reset() noexcept
{
	pos = { 0.0f,7.5f,-18.0f };
	yaw = 0.0f;
}

void Camera::Rotate(float dx) noexcept
{
	yaw = wrap_angle(yaw + dx * rotationSpeed);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	dx::XMStoreFloat3(&translation, dx::XMVector3Transform(
		dx::XMLoadFloat3(&translation),
		dx::XMMatrixRotationRollPitchYaw(0.0f, yaw, 0.0f) *
		dx::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));
	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}