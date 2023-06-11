#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <dxgi.h>


namespace rpf {
	namespace rhi {
		using namespace Microsoft::WRL;


		class RHI {
		public:
			struct INIT_SETTINGS {
				uint32_t width;
				uint32_t height;
				HWND hWnd;
			};

		public:
			explicit RHI(const INIT_SETTINGS& settings) : m_InitSettings(settings) {}
			void Init();
			void Term();
			void Present() { m_D3D11SwapChain->Present(0, 0); }

			ComPtr<ID3D11Device> GetDevice() { return m_D3D11Device; }
			ComPtr<IDXGISwapChain> GetSwapChain() { return m_D3D11SwapChain; }
			ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_D3D11DeviceContext; }

		private:
			const INIT_SETTINGS m_InitSettings;
			
			ComPtr<ID3D11Device> m_D3D11Device;
			ComPtr<IDXGISwapChain> m_D3D11SwapChain;
			ComPtr<ID3D11DeviceContext> m_D3D11DeviceContext;
		};


	}
}