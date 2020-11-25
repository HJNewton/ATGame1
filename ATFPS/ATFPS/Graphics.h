#pragma once
//#include "Window.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

class Graphics
{
	friend class Bindable;

	public:
		Graphics(HWND hWnd);
		Graphics(const Graphics*) = delete;
		Graphics& operator = (const Graphics&) = delete;
		~Graphics() = default;
		void EndFrame();
		void ClearBuffer(float red, float green, float blue); 
		void SetCam(DirectX::FXMMATRIX cam) noexcept;
		DirectX::XMMATRIX GetCam() const noexcept;
		void DrawIndexed(UINT count) noexcept;
		void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		DirectX::XMMATRIX GetProjection() const noexcept;

		
	private:
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX camera;

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};