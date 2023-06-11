#pragma once

#define ASSERT(x) do { if (!((x))) __debugbreak(); } while(0)

#define WINAPI_CHECK(x) do { if (x != S_OK) ASSERT(false); } while(0);

#define D3D_CHECK(x) WINAPI_CHECK(x)