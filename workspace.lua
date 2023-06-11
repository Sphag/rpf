workspace (WORKSPACE_NAME)
    startproject (START_PROJECT)
    configurations {
        "Debug",
        "Profile",
        "Release"
    }

    platforms {
        "x64"
    }

    filter "configurations:Debug"
        runtime "debug"
        symbols "on"
        optimize "off"
    filter "configurations:Profile"
        runtime  "release"
        symbols  "on"
        optimize "on"
    filter "configurations:Release"
        runtime  "release"
        symbols  "off"
        optimize "on"
    filter "platforms:x64"
        architecture "x64"
