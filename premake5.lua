workspace "Mesh-Shaders-Skeletal-Animation"
    architecture "x86_64"

    configurations
    {
        "Debug"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "Application"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "off"

        targetdir("%{wks.location}/build/bin/" .. outputdir .. "-%{prj.name}")
        objdir ("%{wks.location}/build/obj/" .. outputdir .. "-%{prj.name}")

        files
        {
            "src/**.h",
            "src/**.cpp",
            "deps/glad/src/glad.c"
        }

        includedirs
        {
            "%{wks.location}/deps/glad/include",
            
            "D:/Library/GLFW/glfw-3.3.4/include",
            "D:/Library/glm"
        }

        libdirs
        {
            "D:/Library/GLFW/glfw-3.3.4.bin.WIN64/lib-vc2019"
        }

        links
        {
            "opengl32.lib",
            "glfw3.lib"
        }

        filter "configurations:Debug"
            symbols "on"
            defines "DEBUG"
