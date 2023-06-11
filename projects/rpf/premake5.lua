project "rpf"
    kind          "WindowedApp"
    language      "C++"
    cppdialect    "C++17"
    staticruntime "on"
    systemversion "latest"
    warnings      "extra"
    targetdir     (BIN_DIR)
    objdir        (OBJ_DIR)
    files {
        "src/**.cpp",
        "include/**.h",
    }
    includedirs {
        "include",
        ROOT_PATH "dependencies/DirectXMath/Inc",
        ROOT_PATH "dependencies/imgui",
        ROOT_PATH "dependencies/imgui/backends",
    }

    links {
        "d3d11.lib",
        "dxgi.lib",
        "dxguid.lib",
        "uuid.lib",
        "kernel32.lib",
        "user32.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",

        "DirectXMath",
        "imgui",
    }
    defines {

    }

    filter "action:vs*"  -- for Visual Studio actions
        pchheader "pch.h"
        pchsource "src/pch.cpp"
    filter "action:not vs*"  -- for everything else
        pchheader "pch.h"
    filter ""