project "Demo"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    defines 'SOLUTION_DIR=R"($(SolutionDir))"'

    targetdir("%{wks.location}/build/bin/" .. outputdir .. "-%{prj.name}")
    objdir ("%{wks.location}/build/obj/" .. outputdir .. "-%{prj.name}")

    files
    {
        "%{wks.location}/src/Demo/**.h",
        "%{wks.location}/src/Demo/**.cpp",
        "%{wks.location}/deps/meshoptimizer/src/**.h",
        "%{wks.location}/deps/meshoptimizer/src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/src/Engine",

        "%{wks.location}/deps/glad/include",
        "%{wks.location}/deps/spdlog/include",
        "%{wks.location}/deps/imgui",
        
        "D:/Library/GLFW/glfw-3.3.4/include",
        "D:/Library/glm",
        "D:/Library/Assimp/include"
    }

    links
    {
        "Engine"
    }

    filter "configurations:Debug"
        symbols "on"
        defines "DEBUG"