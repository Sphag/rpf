#pragma once

#include <string>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <rend/renderer.h>


namespace rpf {
	namespace app {


		class APPLICATION {
		public:
			struct INIT_SETTINGS {
				std::wstring windowClassName;
				std::wstring windowName;
				HINSTANCE hInstance;
				int nCmdShow;
				int wndWidth;
				int wndHeight;
			};

		public:
			explicit APPLICATION(const INIT_SETTINGS& settings) : m_AppInitSettings(settings) {}
			void Init();
			void Term();
			void Run();

		private:
			void InitWindow();

			static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			void SetDestroy() { m_IsDestroyed = true; }

		private:
			const INIT_SETTINGS m_AppInitSettings;
			std::shared_ptr<rend::RENDERER> m_Renderer = nullptr;

			HWND m_hWindow = NULL;

			bool m_IsDestroyed = false;
		};


	}
}