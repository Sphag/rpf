#pragma once

#include <common.h>

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>


#define IM_ASSERT(x) ASSERT(x)
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>


#include <DirectXMath.h>