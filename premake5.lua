workspace "Mesh-Shaders-Skeletal-Animation"
    architecture "x86_64"
    startproject "Demo"

    configurations
    {
        "Debug"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "src/Engine/premake5.lua"
    include "src/Demo/premake5.lua"
