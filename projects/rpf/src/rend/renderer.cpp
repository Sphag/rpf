#include <pch.h>

#include <rend/renderer.h>


namespace rpf {
	namespace rend {


		VIEWPORT_PASS::VIEWPORT_PASS(std::shared_ptr<rhi::RHI> rhi, const INIT_SETTINGS& settings) :
			RENDER_PASS(rhi, settings),
			m_CurrentViewportRegionAvailable(static_cast<float>(settings.rtWidth), static_cast<float>(settings.rtHeight))
		{
			CreateResourcesAndViews(rhi);

			std::filesystem::path shaderPath = "shaders/basic_triangle.fx";
			std::ifstream file(shaderPath);
			std::string content;
			ComPtr<ID3DBlob> vertexShader;
			ComPtr<ID3DBlob> vertexShaderErr;
			ComPtr<ID3DBlob> pixelShader;
			ComPtr<ID3DBlob> pixelShaderErr;
			if (file.is_open() && file.good()) {
				std::stringstream buffer;
				buffer << file.rdbuf();
				content = buffer.str();
				HRESULT hr = D3DCompile(content.data(), content.size(), 0, 0, 0, "VS", "vs_4_0", 0, 0, &vertexShader, &vertexShaderErr);
				//char* a = (char*)vertexShaderErr->GetBufferPointer();
				D3D_CHECK(hr);
				D3D_CHECK(D3DCompile(content.data(), content.size(), 0, 0, 0, "PS", "ps_4_0", 0, 0, &pixelShader, &pixelShaderErr));

				D3D_CHECK(rhi->GetDevice()->CreateVertexShader(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), nullptr, &m_VertexShader));
				D3D_CHECK(rhi->GetDevice()->CreatePixelShader(pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(), nullptr, &m_PixelShader));
			} else {
				ASSERT(false);
			}

			if (file.is_open()) {
				file.close();
			}

			Vertex vertices[] = {
				
				 Vertex( 0.0f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
				 Vertex( 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
				 Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
			};

			D3D11_BUFFER_DESC vertexBufferDesc{};

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(vertices);
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData{};
			vertexBufferData.pSysMem = vertices;
			D3D_CHECK(rhi->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_TriangleVertBuffer));

			D3D_CHECK(rhi->GetDevice()->CreateInputLayout(layout, 2, vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &m_InputLayout));
		}


		void VIEWPORT_PASS::CreateResourcesAndViews(std::shared_ptr<rhi::RHI> rhi)
		{
			D3D11_TEXTURE2D_DESC desc{};
			desc.Width = static_cast<uint32_t>(m_CurrentViewportRegionAvailable.x);
			desc.Height = static_cast<uint32_t>(m_CurrentViewportRegionAvailable.y);
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			desc.CPUAccessFlags = 0;

			D3D_CHECK(rhi->GetDevice()->CreateTexture2D(&desc, nullptr, &m_Texture));
			D3D_CHECK(rhi->GetDevice()->CreateRenderTargetView(m_Texture.Get(), nullptr, &m_RenderTargetView));
			D3D_CHECK(rhi->GetDevice()->CreateShaderResourceView(m_Texture.Get(), nullptr, &m_ShaderResourceView));
		}


		void VIEWPORT_PASS::Execute(std::shared_ptr<rhi::RHI> rhi)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Viewport");
			
			if (CheckAndRecreateRenderTarget(rhi)) {
				rhi->GetDeviceContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
				rhi->GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), (float*)&m_InitSettings.clearValue);
				rhi->GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
				rhi->GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);

				uint32_t stride = sizeof(Vertex);
				uint32_t offset = 0;
				rhi->GetDeviceContext()->IASetVertexBuffers(0, 1, m_TriangleVertBuffer.GetAddressOf(), &stride, &offset);
				rhi->GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
				rhi->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				D3D11_VIEWPORT viewport{};

				viewport.TopLeftX = 0;
				viewport.TopLeftY = 0;
				viewport.Width = m_CurrentViewportRegionAvailable.x;
				viewport.Height = m_CurrentViewportRegionAvailable.y;

				rhi->GetDeviceContext()->RSSetViewports(1, &viewport);
				rhi->GetDeviceContext()->Draw(3, 0);
				ImGui::Image(static_cast<ImTextureID>(m_ShaderResourceView.Get()), m_CurrentViewportRegionAvailable);
			}


			
			ImGui::End();
			ImGui::PopStyleVar();
			ImVec2 viewportRegionAvailable = ImGui::GetContentRegionAvail();
		}


		bool VIEWPORT_PASS::CheckAndRecreateRenderTarget(std::shared_ptr<rhi::RHI> rhi)
		{
			ImVec2 view = ImGui::GetContentRegionAvail();

			if (static_cast<uint32_t>(view.x) != static_cast<uint32_t>(m_CurrentViewportRegionAvailable.x) ||
				static_cast<uint32_t>(view.y) != static_cast<uint32_t>(m_CurrentViewportRegionAvailable.y)) {
				if (view.x <= 0 || view.y <= 0) {
					return true;
				}

				m_CurrentViewportRegionAvailable = view;
				CreateResourcesAndViews(rhi);
			}

			return true;
		}


		void IMGUI_PASS::Execute(std::shared_ptr<rhi::RHI> rhi)
		{		
			ImGui::Render();

			ComPtr<ID3D11Texture2D> pBackBuffer;
			D3D_CHECK(rhi->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));

			D3D_CHECK(rhi->GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_RenderTargetView));

			rhi->GetDeviceContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
			rhi->GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), (float*)&m_InitSettings.clearValue);

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}


		void RENDERER::Init()
		{
			rhi::RHI::INIT_SETTINGS rhiSettings;
			rhiSettings.hWnd = m_InitSettings.hWnd;
			rhiSettings.width = m_InitSettings.width;
			rhiSettings.height = m_InitSettings.height;
			
			m_RHI = std::make_shared<rhi::RHI>(rhiSettings);
			m_RHI->Init();

			VIEWPORT_PASS::INIT_SETTINGS viewPortPassSettings;
			viewPortPassSettings.name = "Viewport pass";
			viewPortPassSettings.clearValue = { 0.0f, 0.0f, 0.0f, 0.7f };
			viewPortPassSettings.rtWidth = 600;
			viewPortPassSettings.rtHeight = 600;
			m_RenderPasses.emplace_back(std::make_shared<VIEWPORT_PASS>(m_RHI, viewPortPassSettings));

			IMGUI_PASS::INIT_SETTINGS imguiPassSettings;
			imguiPassSettings.name = "ImGui pass";
			imguiPassSettings.clearValue = { 0.2f, 0.3f, 0.7f, 1.0f };

			// imgui renders to back buffer so it doesnt matter what size we pass
			imguiPassSettings.rtWidth = 0;
			imguiPassSettings.rtHeight = 0;
			m_RenderPasses.emplace_back(std::make_shared<IMGUI_PASS>(m_RHI, imguiPassSettings));
		}


		void RENDERER::Term()
		{
			m_RHI->Term();
		}

		void RENDERER::RenderFrame()
		{
			for (const auto& renderPass : m_RenderPasses) {
				renderPass->Execute(m_RHI);
			}
		}


		void RENDERER::Present()
		{
			m_RHI->Present();
		}


	}
}