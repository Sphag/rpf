ROOT_PATH = function (path)
    return string.format("%s/%s", _MAIN_SCRIPT_DIR, path)
end

PROJ_DIR = ROOT_PATH "projects"
DEP_DIR = ROOT_PATH "dependencies"
BIN_DIR = ROOT_PATH "bin/%{cfg.buildcfg}-%{cfg.platform}/%{prj.name}"
OBJ_DIR  = ROOT_PATH "bin/obj/%{cfg.buildcfg}-%{cfg.platform}/%{prj.name}"

WORKSPACE_NAME = "rpf"
START_PROJECT  = "rpf"

include "workspace.lua"
include "dependencies.lua"
include "projects.lua"

group "dependencies"
    for _, path in ipairs(DEPENDENCIES) do
        include(path .. "/premake5.lua")
    end
group ""

for _, path in ipairs(PROJECTS) do
    include(path .. "/premake5.lua")
end