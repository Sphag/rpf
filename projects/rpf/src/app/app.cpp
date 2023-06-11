#include <pch.h>

#include <app/app.h>
#include <rhi/rhi.h>


namespace rpf {
	namespace app {


		void APPLICATION::Init()
		{
			InitWindow();

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

			ImGui::StyleColorsDark();

			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			ImGui_ImplWin32_Init(m_hWindow);

			rend::RENDERER::INIT_SETTINGS settings;
			settings.hWnd = m_hWindow;
			settings.width = m_AppInitSettings.wndWidth;
			settings.height = m_AppInitSettings.wndHeight;
			m_Renderer = std::make_shared<rend::RENDERER>(settings);
			m_Renderer->Init();
		}


		void APPLICATION::InitWindow()
		{
			WNDCLASS wc = {};
			wc.lpfnWndProc = APPLICATION::WindowProc;
			wc.hInstance = m_AppInitSettings.hInstance;
			wc.lpszClassName = m_AppInitSettings.windowClassName.c_str();

			RegisterClass(&wc);

			m_hWindow = CreateWindowEx(
				0,
				m_AppInitSettings.windowClassName.c_str(),
				m_AppInitSettings.windowName.c_str(),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, m_AppInitSettings.wndWidth, m_AppInitSettings.wndHeight,
				NULL,
				NULL,
				m_AppInitSettings.hInstance,
				this
			);

			ASSERT(m_hWindow != NULL);

			ShowWindow(m_hWindow, m_AppInitSettings.nCmdShow);
		}


		LRESULT CALLBACK APPLICATION::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
				return true;
			}

			APPLICATION* thisInstance = (APPLICATION*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			switch (uMsg) {
			case WM_NCCREATE: {
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
				break;
			}
			case WM_DESTROY: {
				thisInstance->SetDestroy();
				break;
			}
			}

			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}


		void APPLICATION::Term()
		{
			m_Renderer->Term();

			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			CloseWindow(m_hWindow);
		}


		void APPLICATION::Run()
		{
			while (!m_IsDestroyed) {
				MSG msg = {};
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if (m_IsDestroyed) {
					break;
				}

				ImGui_ImplWin32_NewFrame();
				ImGui_ImplDX11_NewFrame();
				ImGui::NewFrame();

				ImGuiIO& io = ImGui::GetIO();
				ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

				// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				//ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				windowFlags |= ImGuiWindowFlags_NoBackground;

				ImGui::Begin("DockSpace", nullptr, windowFlags);
				ImGui::PopStyleVar(2);

				// Submit the DockSpace
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
				}

				ImGui::End();
				// setup imgui here

				const bool isMinimized = IsIconic(m_hWindow);
				if (!isMinimized) {
					m_Renderer->RenderFrame();
					m_Renderer->Present();
				}

				ImGui::EndFrame();

				// Update and Render additional Platform Windows
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}
			}
		}


	}
}