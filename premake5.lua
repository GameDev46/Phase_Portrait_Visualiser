workspace "Phase_Portrait_Visualiser"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    startproject "system-solver"

project "phase-plotter"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "src/**.cpp",
        "src/**.h",
        "external/imgui/imgui*.cpp",
        "external/imgui/misc/cpp/imgui_stdlib.cpp",
    }

    includedirs {
        ".",
        "external",
        "external/imgui",
        "external/imgui/backends",
        "external/raylib/include"
    }

    libdirs {
        "external/raylib/lib"
    }

    links {
        "raylib",
        "opengl32",
        "gdi32",
        "winmm"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
