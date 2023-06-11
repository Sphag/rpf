#include <pch.h>

#include <rhi/rhi.h>


namespace rpf {
	namespace rhi {


		void RHI::Init()
		{	
			ComPtr<IDXGIFactory1> pDXGIFactory;
			D3D_CHECK(CreateDXGIFactory1(IID_PPV_ARGS(&pDXGIFactory)));

			ComPtr<IDXGIAdapter1> pDXGIAdapter;
			DXGI_ADAPTER_DESC1 adapterDesc;
			ComPtr<IDXGIAdapter1> pDXGICurrentAdapter;
			for (uint32_t i = 0; pDXGIFactory->EnumAdapters1(i, &pDXGICurrentAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
				DXGI_ADAPTER_DESC1 adapterCurDesc{};
				D3D_CHECK(pDXGICurrentAdapter->GetDesc1(&adapterCurDesc));

				ComPtr<IDXGIOutput> pDXGIOutput;
				for (uint32_t j = 0; pDXGICurrentAdapter->EnumOutputs(j, &pDXGIOutput) != DXGI_ERROR_NOT_FOUND; j++) {
					DXGI_OUTPUT_DESC outputDesc;
					D3D_CHECK(pDXGIOutput->GetDesc(&outputDesc));
				}

				if (!pDXGIOutput && (!pDXGIAdapter || adapterCurDesc.DedicatedVideoMemory > adapterDesc.DedicatedSystemMemory)) {
					pDXGIAdapter = pDXGICurrentAdapter;
					adapterDesc = adapterCurDesc;
				}
			}

			DXGI_MODE_DESC bufferDesc{};
			bufferDesc.Width = m_InitSettings.width;
			bufferDesc.Height = m_InitSettings.height;
			bufferDesc.RefreshRate.Numerator = 60;
			bufferDesc.RefreshRate.Denominator = 1;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			DXGI_SWAP_CHAIN_DESC swapChainDesc{};
			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;
			swapChainDesc.OutputWindow = m_InitSettings.hWnd;
			swapChainDesc.Windowed = true;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

			D3D_CHECK(D3D11CreateDeviceAndSwapChain(
				pDXGIAdapter.Get(),
				D3D_DRIVER_TYPE_UNKNOWN,
				nullptr,
				D3D11_CREATE_DEVICE_DEBUG,
				0u, 0u,
				D3D11_SDK_VERSION,
				&swapChainDesc,
				&m_D3D11SwapChain,
				&m_D3D11Device,
				nullptr,
				&m_D3D11DeviceContext)
			);

			ImGui_ImplDX11_Init(m_D3D11Device.Get(), m_D3D11DeviceContext.Get());
		}


		void RHI::Term()
		{
			ImGui_ImplDX11_Shutdown();
		}


	}
}