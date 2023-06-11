#pragma once

#include <rhi/rhi.h>


namespace rpf {
	namespace rend {
		using namespace Microsoft::WRL;


		class RENDER_PASS {
		public:
			struct INIT_SETTINGS {
				std::string name;
				D3DCOLORVALUE clearValue;
				std::vector<ComPtr<ID3D11ShaderResourceView>> shaderResourceViews;
				uint32_t rtWidth;
				uint32_t rtHeight;
			};

		public:
			RENDER_PASS(std::shared_ptr<rhi::RHI> rhi, const INIT_SETTINGS& settings) : m_InitSettings(settings) {}

			//virtual void Prepare(std::shared_ptr<rhi::RHI> rhi) = 0;
			virtual void Execute(std::shared_ptr<rhi::RHI> rhi) = 0;

			virtual ~RENDER_PASS() {}

		protected:
			const INIT_SETTINGS m_InitSettings;
			ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
			bool m_IsPrepared = false;
		};


		class IMGUI_PASS : public RENDER_PASS {
		public:
			IMGUI_PASS(std::shared_ptr<rhi::RHI> rhi, const INIT_SETTINGS& settings) : RENDER_PASS(rhi, settings) {}
			//void Prepare(std::shared_ptr<rhi::RHI> rhi) override;
			void Execute(std::shared_ptr<rhi::RHI> rhi) override;

			virtual ~IMGUI_PASS() {}
		};


		struct Vertex 
		{
			Vertex() {}
			Vertex(float x, float y, float z,
				float cr, float cg, float cb, float ca)
				: pos(x, y, z), color(cr, cg, cb, ca) {}

			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
		};

		inline D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};


		class VIEWPORT_PASS : public RENDER_PASS {
		public:
			VIEWPORT_PASS(std::shared_ptr<rhi::RHI> rhi, const INIT_SETTINGS& settings);
			void Execute(std::shared_ptr<rhi::RHI> rhi) override;

			virtual ~VIEWPORT_PASS() {}

		private:
			void CreateResourcesAndViews(std::shared_ptr<rhi::RHI> rhi);

		private:
			bool CheckAndRecreateRenderTarget(std::shared_ptr<rhi::RHI> rhi);

		private:
			ComPtr<ID3D11Texture2D> m_Texture;
			ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
			ComPtr<ID3D11VertexShader> m_VertexShader;
			ComPtr<ID3D11PixelShader> m_PixelShader;
			ComPtr<ID3D11Buffer> m_TriangleVertBuffer;
			ComPtr<ID3D11InputLayout> m_InputLayout;
			
			ImVec2 m_CurrentViewportRegionAvailable;
		};


		class RENDERER {
		public:
			struct INIT_SETTINGS {
				HWND hWnd;
				uint32_t width;
				uint32_t height;
			};

		public:
			explicit RENDERER(const INIT_SETTINGS& settings) : m_InitSettings(settings) {}
			void Init();
			void Term();

			void RenderFrame();
			void Present();
		private:
		private:
			const INIT_SETTINGS m_InitSettings;
			std::shared_ptr<rhi::RHI> m_RHI = nullptr;
			std::vector<std::shared_ptr<RENDER_PASS>> m_RenderPasses;
		};

	}
}