project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    defines 'SOLUTION_DIR=R"($(SolutionDir))"'

    targetdir("%{wks.location}/build/bin/" .. outputdir .. "-%{prj.name}")
    objdir ("%{wks.location}/build/obj/" .. outputdir .. "-%{prj.name}")

    files
    {
        "%{wks.location}/src/Engine/**.h",
        "%{wks.location}/src/Engine/**.cpp",
        "%{wks.location}/deps/glad/src/glad.c",
        "%{wks.location}/deps/imgui/**.h",
        "%{wks.location}/deps/imgui/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/deps/glad/include",
        "%{wks.location}/deps/spdlog/include",
        "%{wks.location}/deps/imgui",
        
        "D:/Library/GLFW/glfw-3.3.4/include",
        "D:/Library/glm",
        "D:/Library/Assimp/include"
    }

    libdirs
    {
        "D:/Library/GLFW/glfw-3.3.4.bin.WIN64/lib-vc2019",
        "D:/Library/Assimp/lib/Debug"
    }

    links
    {
        "opengl32.lib",
        "glfw3.lib",
        "assimp-vc142-mtd.lib"
    }

    filter "configurations:Debug"
        symbols "on"
        defines "DEBUG"
