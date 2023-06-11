
project "imgui"
    kind          "StaticLib"
    language      "C++"
    cppdialect    "C++17"
    staticruntime "on"
    systemversion "latest"
    warnings      "extra"
    targetdir     (BIN_DIR)
    objdir        (OBJ_DIR)
    files {
        "imconfig.h",
        "imgui.h",
        "imgui_internal.h",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",

        "imgui_widgets.cpp",
        "imgui_tables.cpp",
        "imgui_draw.cpp",
        "imgui_demo.cpp",
        "imgui.cpp",

-- backends, subject to change
        "backends/imgui_impl_dx11.h",
        "backends/imgui_impl_dx11.cpp",

        "backends/imgui_impl_win32.cpp",
        "backends/imgui_impl_win32.h",
    }
    includedirs {
        "./",
    }
    links {

    }
    defines {

    }
